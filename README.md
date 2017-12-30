# PS4 Linux Loader
(Doesn't work YET).

A simple payload that let you run Linux on your 4.05 PS4

## How to build
I use https://github.com/idc/ps4-payload-sdk to compile it. You also need to compile https://github.com/fail0verflow/ps4-kexec and place 'kexec.bin' into this folder. Compile kexec with 'make CFLAG='-DPS4_4_05 -DKASLR -DNO_SYMTAB'.

## How to use
You need a FAT32 formatted USB drive plugged in on any PS4's USB port with the following files on the root directory:

bzImage : Kernel image that will be loaded. Recommended to use this sources to compile it.

initramfs.cpio.gz : The initial file system that gets loaded into memory during the Linux startup process. This one is recommended.

## Credits and links
Thanks to kR105 and the team fail0verflow

https://github.com/kR105-zz/PS4-dlclose/tree/linux-loader

https://cturt.github.io/ps4-3.html

https://github.com/CTurt/PS4-playground
