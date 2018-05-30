# PS4 Linux Loader
A simple payload that let you run Linux on your 5.05 PS4

## How to build
I use https://github.com/valentinbreiz/ps4-payload-sdk to compile it. You also need to compile https://github.com/fail0verflow/ps4-kexec and place 'kexec.bin' into this folder. Compile kexec with 'make CFLAG='-DPS4_5_05 -DKASLR -DNO_SYMTAB'.

## How to use

You need a FAT32 formatted USB drive plugged in on any PS4's USB port with the following files on the root directory : bzImage and initramfs.cpio.gz. You can download [them here](https://mega.nz/#!hEh1QI4B!gCDA5l7GyTekQ-fURvKw6WRieSbHETb3tYHb--SkmhM).

Then you will need to send the payload (PS4-Linux-Loader.bin) to your PS4. For that go to your PS4 web browser, go to darbness.com/ps4 and send the payload to your PS4 using netcat or other.. (You can also use my tool: [PS4 Payload Sender](https://github.com/valentinbreiz/PS4-Payload-Sender)).

For 4.05:

https://github.com/valentinbreiz/PS4-Linux-Loader/tree/master

For 4.55:

https://github.com/valentinbreiz/PS4-Linux-Loader/tree/4.55

For 5.01:

https://github.com/valentinbreiz/PS4-Linux-Loader/tree/5.01

For PS4 Pro / Slim / FAT:

https://github.com/eeply/ps4-linux/tree/ps4pro

## Credits and links
Thanks to 2much4u, Darbnes and jiangwei.

Useful links:

- For the kexec execution: https://github.com/kR105-zz/PS4-dlclose/tree/linux-loader

- For kexec: https://github.com/fail0verflow/ps4-kexec

- For more explanations: https://cturt.github.io/ps4-3.html

- For executing code in kernel space: https://github.com/VV1LD/405-KernelDumper
