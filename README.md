# Terminal Root OS
A basic Operating System **32-bit**, just the kernel, that prints text directly to the **VGA** and restarts when you press *[ENTER]*.

![Terminal Root OS running in QEMU](./qemu-running-terminalroot-os.png)

## Made with C++ and GNU Assembler

---

## Dependencies
> Example on Ubuntu
```bash
sudo apt install build-essential
```
> C++ Compiler(`g++`), `as`, `ld` and `make`.

Use QEMU to test. Install QEMU:
```bash
sudo apt install qemu-kvm
```

---

## Compiling and testing

Compiling:
```bash
git clone https://github.com/terroo/terminalroot-os
cd terminalroot-os
make
```

Testing:
```bash
qemu-system-i386 -kernel terminal-root-os.bin
```
> Or `qemu-system-x86-64 -kernel terminalroot-os.bin`.

If you want to add to a GRUB Legacy (does not work in UEFI):
> Edit: `sudo vim /boot/grub/grub.cfg` and move binary: `sudo mv terminalroot-os.bin /boot/`.

Add new line:
```bash
### BEGIN TERMINALROOTOS

menuentry 'Terminal Root OS' {
  multiboot /boot/terminal-root-os.bin
  boot
}

### END TERMINALROOTOS
```

---

### [Watch video](https://youtu.be/3GwRF0IK1Ks)
