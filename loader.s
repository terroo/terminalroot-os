.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern kmain # O uso de -fno-leading-underscore evita iniciar o underline, ex.: _kmain
.global loader

loader:
  mov $kernel_stack, %esp
  push %eax
  push %ebx
  call kmain

_stop:
  cli
  hlt
  jmp _stop


.section .bss
.space 2*1024*1024; # 2MiB
kernel_stack:
