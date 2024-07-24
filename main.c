// Mentioned in the spec
#define MAGIC_NUMBER 0x1BADB002

// Protected Mode
#define MAGIC_FLAGS 0

typedef struct multiboot_header {
     long magic;
     long mode;
     long checksum;
} multiboot_header_t;

multiboot_header_t __attribute__((section(".multiboot"))) header = {
     .magic = MAGIC_NUMBER, 
     .mode = MAGIC_FLAGS, 
     
     // Checksum should return 0 when added to the sum of magic and flags
     .checksum = -(MAGIC_NUMBER + MAGIC_FLAGS),
};

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
          vga_buffer[i+1] = 0x02;
     }
     

     for(;;);
}