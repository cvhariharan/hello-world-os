## Hello World OS
A minimal kernel the prints "Hello World!" in under 50 lines of C.

### Build Instructions
The first step is to build the object file from `main.c`.
```bash
gcc -m32 -nostdlib -fno-stack-protector -ffreestanding -c main.c -o a.o
```
- `m32` - compile 32-bit binary
- `nostdlib` - don't link libc
- `-fno-stack-protector` - don't add stack protector
- `-ffreestanding` - compile freestanding binary

Next we use `ld` to create the final `kernel.bin`.
```bash
ld -m elf_i386 -nmagic --output=kernel.bin --script=linker.ld a.o
```

The `kernel.bin` can be booted using an i386 machine emulated using QEMU.
```bash
qemu-system-i386 -kernel kernel.bin
```

All these commands can also be executed using `make`.

## Presentation

Navigate to `presentation` folder and run
```
npm i
npm run dev
```