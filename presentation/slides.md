---
# You can also start simply with 'default'
theme: seriph
colorSchema: light
# random image from a curated Unsplash collection by Anthony
# like them? see https://unsplash.com/collections/94734566/slidev
background: assets/images/cover.jpg
# some information about your slides (markdown enabled)
title: Hello World in 50 LoC
info: Building a minimal kernel in C
class: text-center
# https://sli.dev/features/drawing
drawings:
  persist: false
# slide transition: https://sli.dev/guide/animations.html#slide-transitions
transition: slide-left
# enable MDC Syntax: https://sli.dev/features/mdc
mdc: true
---

# Hello World in 50 LoC

Building a minimal kernel in C

---
transition: fade-out
layout: center
---
# Let's start from the beginning

```mermaid {scale: 2.0, alt: 'boot sequence'}
flowchart LR
  BIOS
  Bootloader
  Kernel
  BIOS --> Bootloader --> Kernel
```
---
transition: fade-out
---

# BIOS

<v-click>
<ul>
  <li> Part of the hardware </li>
  <li> POST, low-level drivers, finding boot disk </li>
</ul>
</v-click>

<v-click>


## Finding the boot disk
 
- MBR formatted disk
- Magic number (`0x55AA`) in the last 2 bytes of the first sector
- Control transferred to valid bootloader

</v-click>

<!--
- Fun fact: GPUs also have a bios which is loaded by the motherboard bios during boot. The VGA bios is also identified using the magic number.
- Bios is responsible for setting up the primary GPU in case of multi GPU systems. This gets mapped to 0xb8000 address.
-->
---
transition: fade-out
---

# Bootloader

- Transitioning to Protected Mode (32-bit)
  - Allows access to 4GB RAM
  - Allows Paging
  - Hardware level memory protection using GDT
  - Interrupts
  - Loads the kernel


---
transition: slide-up
level: 2
---
# Multiboot complaint bootloaders

- Specific header in the first 8K bytes of the binary
- Allows the kernel to guide the bootloader to setup the environment

<img style="width:50%" src="/assets/images/header.png"/>

<!--
- Spec maintained by FSF
- We will work with v1
- Mandatory 12 bytes (3 words)
-->
---
transition: fade-out
---
# Header

````md magic-move {lines: true}
```c {*|2|4|6-10|*}
// Mentioned in the spec
#define MAGIC_NUMBER 0x1BADB002

#define MAGIC_FLAGS 0

typedef struct multiboot_header {
     long magic;
     long mode;
     long checksum;
} multiboot_header_t;
```

```c {*|1|*}
multiboot_header_t __attribute__((section(".multiboot"))) header = {
     .magic = MAGIC_NUMBER, 
     .mode = MAGIC_FLAGS, 
     
     // Checksum should return 0 when added to the sum of magic and flags
     .checksum = -(MAGIC_NUMBER + MAGIC_FLAGS),
};
```
````

### Links
- https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
---
transition: fade-out
---

# Printing to Screen

````md magic-move {lines: true}
```c {*|2|11-15|*}
void _start() {
     char *vga_buffer = (char *) 0xb8000;
     const char *str = "Hello World!";

     // Clear screen
     for(int i = 0; i < 80*25; i=i+2) {
          vga_buffer[i] = ' ';
          vga_buffer[i+1] = 0x00;
     }

     // Write characters to video memory
     for(int i = 0; *str != 0; i+=2) {
          vga_buffer[i] = *str++;
          vga_buffer[i+1] = 0x02; //fg: green
     }
     

     for(;;);
}
```
````  
<!--
- This is called VGA Mode 3. Linear access to the video memory.
- The fonts are from the bios.
- In real-mode, bios functions can be used by triggering software interrupts to print text on screen.
-->
### Links
- https://wiki.osdev.org/Text_UI
---
transition: fade-out
---

# Linker
Linker script [^1]
````md magic-move {lines: true}
```text {*|3|4-8|*}
ENTRY(_start)
SECTIONS {
	. = 1M;
    .text :
    {
        *(.multiboot)
        *(.text)
    }
    .rodata :
	{
		*(.rodata)
	}
	.data :
	{
		*(.data)
	}
	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
```
````

[^1]: https://mcyoung.xyz/2021/06/01/linker-script/
<!--

- .text is where executable code goes.
- .rodata is read only data; it is where global constants are placed.
- .data is where global variables that are initialized at compile time are placed.
- .bss is where uninitialized global variables are placed.

-->
---
transition: fade-out
layout: center
zoom: 1.5
---

````md magic-move {lines: true}
```bash {*|2|3|4|*}
gcc -m32 \
     -nostdlib \
     -ffreestanding \
     -c main.c -o a.o
```

```bash {*|1|2|3|*}
ld -m elf_i386 \
     -nmagic \
     --output=kernel.bin --script=linker.ld a.o
```
````
<!--
- -ffreestanding denotes a bare-metal environment.
  - Only few standard headers are available, mostly just types.
  - execution doesn't have to begin at main.
  - prevents gcc optimizations that are based on hosted environment assumptions
- -nostdlib is required because even in freestanding mode, gcc can emit calls to memcpy or memset (for allocations) which are part of stdlib. 
- -c compile and assemble but don't link
-->
---
transition: fade-out
layout: center
---

# DEMO

---
transition: fade-out
layout: center
---

# Links
- https://github.com/cvhariharan/hello-world-os
- https://wiki.osdev.org/Expanded_Main_Page
- https://intermezzos.github.io/book/first-edition/preface.html
- https://os.phil-opp.com/
- https://github.com/cvhariharan/WindmillOS