#define DEBUG_SOCKET 1

#include "ps4.h"
#include "defines.h"

extern char kexec[];
extern unsigned kexec_size;

static int sock;

void usbthing();

int _main(void) {

	initKernel();	
	initLibc();
	initNetwork();

	struct sockaddr_in server;

	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 1, 12);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	int flag = 1;
	sceNetSetsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

	printfsocket("Kexec init...\n");

	void *DT_HASH_SEGMENT = (void *)0xffffffff82200160;
	
	printfsocket("Kexec init [0]\n");

	printfsocket("Kexec size [%d]\n", kexec_size);
	
	memcpy(DT_HASH_SEGMENT, kexec, kexec_size); //Fails here
	
	printfsocket("Kexec init [1]\n");

	void (*kexec_init)(void *, void *) = DT_HASH_SEGMENT;

	printfsocket("Kexec init [2]\n");

	kexec_init(NULL, NULL);
	
	printfsocket("Starting PS4 Linux Loader\n");
	
	usbthing();

	printfsocket("Done!\n");
	sceNetSocketClose(sock);

	return 0;
}

void usbthing()
{
	// Open bzImage file from USB
	FILE *fkernel = fopen("/mnt/usb0/bzImage", "r");
	fseek(fkernel, 0L, SEEK_END);
	int kernelsize = ftell(fkernel);
	fseek(fkernel, 0L, SEEK_SET);

	// Open initramfs file from USB
	FILE *finitramfs = fopen("/mnt/usb0/initramfs.cpio.gz", "r");
	fseek(finitramfs, 0L, SEEK_END);
	int initramfssize = ftell(finitramfs);
	fseek(finitramfs, 0L, SEEK_SET);

	printfsocket("kernelsize = %d\n", kernelsize);
	printfsocket("initramfssize = %d\n", initramfssize);

	// Sanity checks
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

	// Call sys_kexec
	syscall(153, kernel, kernelsize, initramfs, initramfssize, cmd_line);

	free(kernel);
	free(initramfs);

	// Reboot PS4
	int evf = syscall(540, "SceSysCoreReboot");
	syscall(546, evf, 0x4000, 0);
	syscall(541, evf);
	syscall(37, 1, 30);
}
