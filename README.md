# PS4 Linux Loader
(Doesn't work YET).

A simple payload that let you run Linux on your 4.05 PS4

## How to build
I use https://github.com/idc/ps4-payload-sdk to compile it. You also need to compile https://github.com/fail0verflow/ps4-kexec and place 'kexec.bin' into this folder. Compile kexec with 'make CFLAG='-DPS4_4_00 -DKASLR -DNO_SYMTAB'.

## Credits and links
Thanks to kR105 and the team fail0verflow

https://cturt.github.io/ps4-3.html
