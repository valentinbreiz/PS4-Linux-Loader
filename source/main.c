#define DEBUG_SOCKET 1

#include "ps4.h"
#include "defines.h"

#define    KERN_XFAST_SYSCALL 0x3095D0

#define	CTL_KERN	1	/* "high kernel": proc, limits */
#define	KERN_PROC	14	/* struct: process entries */
#define	KERN_PROC_VMMAP	32	/* VM map entries for process */
#define	KERN_PROC_PID	1	/* by process id */

extern char kexec_ps4[];
extern unsigned kexec_size;

static int sock;

void usbthing();

unsigned int long long __readmsr(unsigned long __register) {
	// Loads the contents of a 64-bit model specific register (MSR) specified in
	// the ECX register into registers EDX:EAX. The EDX register is loaded with
	// the high-order 32 bits of the MSR and the EAX register is loaded with the
	// low-order 32 bits. If less than 64 bits are implemented in the MSR being
	// read, the values returned to EDX:EAX in unimplemented bit locations are
	// undefined.
	unsigned long __edx;
	unsigned long __eax;
	__asm__ ("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}

int kpayload(struct thread *td, struct kpayload_args* args){

	//Starting kpayload...
	
	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	//Reading kernel_base...
	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-0x3095D0];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[0x10399B0];
	void** got_rootvnode = (void**)&kernel_ptr[0x21AFA30];
	
	//Resolve kernel functions...
	int (*copyout)(const void *kaddr, void *uaddr, size_t len) = (void *)(kernel_base + 0x14A7B0);
	int (*printfkernel)(const char *fmt, ...) = (void *)(kernel_base + 0x17F30);
	int (*copyin)(const void *uaddr, void *kaddr, size_t len) = (void *)(kernel_base + 0x14A890);

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_prison = *got_prison0;
	fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

	//Disable write protection...
	uint64_t cr0 = readCr0();
	writeCr0(cr0 & ~X86_CR0_WP);
	
	//Kexec init
	void *DT_HASH_SEGMENT = (void *)(kernel_base+ 0xB1D820); //For you @Vultra :P
	memcpy(DT_HASH_SEGMENT,kexec_ps4, kexec_size);

	void (*kexec_init)(void *, void *) = DT_HASH_SEGMENT;

	kexec_init((void *)(kernel_base+0x17F30), NULL);

	// Say hello and put the kernel base in userland to we can use later
	printfkernel("\nPS4 Linux Loader for 4.55 by valentinbreiz\n");

	printfkernel("kernel base is:0x%016llx\n", kernel_base);

	uint64_t uaddr;
	memcpy(&uaddr,&args[2],8);

	printfkernel("uaddr is:0x%016llx\n", uaddr);

	copyout(&kernel_base, uaddr, 8);

	return 0;
}

int _main(struct thread *td) {

	initKernel();	
	initLibc();
	initNetwork();
	initPthread();

	struct sockaddr_in server;

	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 28);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	int flag = 1;
	sceNetSetsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
	
	printfsocket("Connected!\n");

	printfsocket("Hello from valentinbreiz :3\n");

	uint64_t* dump = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	printfsocket("Starting kernel patch...\n");

	int sRet = syscall(11,kpayload,td,dump);

	printfsocket("Kernel patched, Kexec initialized!\n");

	printfsocket("Starting PS4 Linux Loader\n");
	
	usbthing();

	printfsocket("Done!\n");

	sceNetSocketClose(sock);

	return 0;

}

void usbthing()
{

	printfsocket("Open bzImage file from USB\n");
	FILE *fkernel = fopen("/mnt/usb0/bzImage", "r");
	fseek(fkernel, 0L, SEEK_END);
	int kernelsize = ftell(fkernel);
	fseek(fkernel, 0L, SEEK_SET);

	printfsocket("Open initramfs file from USB\n");
	FILE *finitramfs = fopen("/mnt/usb0/initramfs.cpio.gz", "r");
	fseek(finitramfs, 0L, SEEK_END);
	int initramfssize = ftell(finitramfs);
	fseek(finitramfs, 0L, SEEK_SET);

	printfsocket("kernelsize = %d\n", kernelsize);
	printfsocket("initramfssize = %d\n", initramfssize);

	printfsocket("Checks if the files are here\n");
	if(kernelsize == 0 || initramfssize == 0) {
		printfsocket("no file error im dead");
		fclose(fkernel);
		fclose(finitramfs);
		return;
	}

	void *kernel, *initramfs;
	char *cmd_line = "panic=0 clocksource=tsc radeon.dpm=0 console=tty0 console=ttyS0,115200n8 "
			"console=uart8250,mmio32,0xd0340000 video=HDMI-A-1:1920x1080-24@60 "
			"consoleblank=0 net.ifnames=0 drm.debug=0";
	
	kernel = malloc(kernelsize);
	initramfs = malloc(initramfssize);

	printfsocket("kernel = %llp\n", kernel);
	printfsocket("initramfs = %llp\n", initramfs);

	fread(kernel, kernelsize, 1, fkernel);
	fread(initramfs, initramfssize, 1, finitramfs);

	fclose(fkernel);
	fclose(finitramfs);

	//Call sys_kexec (153 syscall)
	syscall(153, kernel, kernelsize, initramfs, initramfssize, cmd_line);

	free(kernel);
	free(initramfs);

	//Reboot PS4
	int evf = syscall(540, "SceSysCoreReboot");
	syscall(546, evf, 0x4000, 0);
	syscall(541, evf);
	syscall(37, 1, 30);

}
