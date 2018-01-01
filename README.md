# PS4 Linux Loader
WORKS! with https://github.com/idc/ps4-entrypoint-405 exploit.
hosted here for your use darbness.com/ps4

A simple payload that let you run Linux on your 4.05 PS4

## How to build
I use https://github.com/idc/ps4-payload-sdk to compile it. You also need to compile https://github.com/fail0verflow/ps4-kexec and place 'kexec.bin' into this folder. Compile kexec with 'make CFLAG='-DPS4_4_05 -DKASLR -DNO_SYMTAB'.

## How to use
You need a FAT32 formatted USB drive plugged in on any PS4's USB port with the following files on the root directory : bzImage and initramfs.cpio.gz. You can download [them here](https://mega.nz/#!hEh1QI4B!gCDA5l7GyTekQ-fURvKw6WRieSbHETb3tYHb--SkmhM).

## To do:

- Fix ASLR (that causes a random successful boot)
- Clean code

## Credits and links
Thanks to kR105, 2much4u, Darbnes and the team fail0verflow

https://github.com/kR105-zz/PS4-dlclose/tree/linux-loader

https://cturt.github.io/ps4-3.html
