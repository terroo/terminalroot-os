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

    const int KEY_P      = 0x19;
    const int KEY_W      = 0x11;
    const int KEY_A      = 0x1E;
    const int KEY_S      = 0x1F; 
    const int KEY_D      = 0x20; 
    const int KEY_UP     = 0x48; 
    const int KEY_DOWN   = 0x50; 
    const int KEY_LEFT   = 0x4B; 
    const int KEY_RIGHT  = 0x4D; 

    uint8_t read_key_scancode();
    uint8_t read_key_nonblocking();

    void reboot_system();
    void printf(const char* str);

    void put_char_at(int x, int y, char c, uint8_t attr);

    void clear_screen_color(uint8_t attr);

    void delay(uint32_t count);

    void run_snake();
    void pit_delay_ms(uint32_t ms);
};
