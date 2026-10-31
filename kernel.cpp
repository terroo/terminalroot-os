#include "kernel.hpp"
#include "gdt.hpp"

class GDT {
public:
    GDTEntry entries[3];
    GDTPointer ptr;

    void init_gdt() {
        set_entry(0, 0, 0, 0, 0);
        set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
        set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
        ptr.limit = (sizeof(GDTEntry) * 3) - 1;
        ptr.base  = (uint32_t)&entries;
        load_gdt((uint32_t)&ptr);
    }

private:
    void set_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
        entries[idx].base_low    = (base & 0xFFFF);
        entries[idx].base_middle = (base >> 16) & 0xFF;
        entries[idx].base_high   = (base >> 24) & 0xFF;
        entries[idx].limit_low   = (limit & 0xFFFF);
        entries[idx].granularity = (limit >> 16) & 0x0F;
        entries[idx].granularity |= gran & 0xF0;
        entries[idx].access      = access;
    }
    void load_gdt(uint32_t gdt_ptr_addr) {
        __asm__ __volatile__(
            "lgdt (%0)            \n\t"
            "ljmp $0x08, $.reload_cs \n\t" 
            ".reload_cs:          \n\t"
            "mov $0x10, %%ax      \n\t" 
            "mov %%ax, %%ds       \n\t"
            "mov %%ax, %%es       \n\t"
            "mov %%ax, %%fs       \n\t"
            "mov %%ax, %%gs       \n\t"
            "mov %%ax, %%ss       \n\t"
            : : "r"(gdt_ptr_addr) : "ax"
        );
    }
};


TerminalRootOS::TerminalRootOS(){
  clear_screen();
  cursor_x = cursor_y = 0;
}

void TerminalRootOS::clear_screen(){
  unsigned short* VideoMemory = reinterpret_cast<unsigned short*>(VGA_MEMORY);
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
    VideoMemory[i] = (VideoMemory[i] & 0xFF00) | ' ';
  }
}

void TerminalRootOS::move_cursor(int x, int y){
  cursor_x = x;
  cursor_y = y;
}

void TerminalRootOS::advance_cursor(){
  cursor_x++;
  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }
  if (cursor_y >= VGA_HEIGHT) {
    cursor_y = VGA_HEIGHT - 1;
  }
}

void TerminalRootOS::put_char(char c){
  unsigned short* VideoMemory = reinterpret_cast<unsigned short*>(VGA_MEMORY);

  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= VGA_HEIGHT) {
      cursor_y = VGA_HEIGHT - 1;
    }
  } else {
    int index = cursor_y * VGA_WIDTH + cursor_x;
    VideoMemory[index] = (VideoMemory[index] & 0xFF00) | c;
    advance_cursor();
  }
}

void TerminalRootOS::printf(const char* str){
  for (int i = 0; str[i] != '\0'; ++i) {
    put_char(str[i]);
  }
}

uint8_t TerminalRootOS::read_key_scancode(){
  uint8_t scancode = 0;
  while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0);
  scancode = inb(KEYBOARD_DATA_PORT);
  return scancode;
}

void TerminalRootOS::reboot_system(){
  outb(KEYBOARD_STATUS_PORT, 0xFE);
}

inline uint8_t TerminalRootOS::inb(uint16_t port){
  uint8_t result;
  asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}
inline void TerminalRootOS::outb(uint16_t port, uint8_t value){
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}
