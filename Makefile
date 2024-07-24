.PHONY: all
all: kernel.bin
	qemu-system-i386 -kernel kernel.bin

a.o: main.c
	gcc -m32 -nostdlib -ffreestanding -c main.c -o a.o

kernel.bin: a.o linker.ld
	ld -m elf_i386 -nmagic --output=kernel.bin --script=linker.ld a.o

.PHONY: presentation
presentation:
	cd presentation && npm run dev