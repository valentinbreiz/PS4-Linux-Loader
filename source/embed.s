    .section .rodata
    .global kexec_ps4
    .type   kexec_ps4, @object
    .align  4
kexec_ps4:
    .incbin "kexec.bin"
kexec_end:
    .global kexec_size
    .type   kexec_size, @object
    .align  4
kexec_size:
    .int    kexec_end - kexec_ps4
