#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

class TerminalRootOS {
  const int VGA_WIDTH = 80;
  const int VGA_HEIGHT = 25;
  const unsigned int VGA_MEMORY = 0xB8000;

  const int KEYBOARD_DATA_PORT = 0x60;
  const int KEYBOARD_STATUS_PORT = 0x64;

  int cursor_x, cursor_y;

  void move_cursor(int x, int y);

  void advance_cursor();

  void put_char(char c);

  void clear_screen();

  inline uint8_t inb(uint16_t port);
  inline void outb(uint16_t port, uint8_t value);

  public:
    TerminalRootOS();
    const int ENTER_SCANCODE = 0x1C;  
    uint8_t read_key_scancode();
    void reboot_system();
    void printf(const char* str);
};

