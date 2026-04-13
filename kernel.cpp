#include "kernel.hpp"

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


// Novas funções-membro

uint8_t TerminalRootOS::read_key_nonblocking() {
  if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
    return 0;
  uint8_t sc = inb(KEYBOARD_DATA_PORT);
  if (sc == 0xE0) {
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0);
    sc = inb(KEYBOARD_DATA_PORT);
  }
  return sc;
}

void TerminalRootOS::put_char_at(int x, int y, char c, uint8_t attr) {
  unsigned short* vga = reinterpret_cast<unsigned short*>(VGA_MEMORY);
  vga[y * VGA_WIDTH + x] = ((uint16_t)attr << 8) | (uint8_t)c;
}

void TerminalRootOS::clear_screen_color(uint8_t attr) {
  unsigned short* vga = reinterpret_cast<unsigned short*>(VGA_MEMORY);
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    vga[i] = ((uint16_t)attr << 8) | ' ';
}

void TerminalRootOS::delay(uint32_t count) {
  for (volatile uint32_t c = count; c != 0; c = c - 1) {
    asm volatile("nop");
  }
}

// Joguinho da Cobra

void TerminalRootOS::run_snake() {
  const int GX = 1, GY = 2;
  const int GW = 40, GH = 20;
  const int MAX_LEN = 100;

  // Cores
  const uint8_t COLOR_BG       = 0x10;
  const uint8_t COLOR_BORDER   = 0x1F;
  const uint8_t COLOR_SNAKE    = 0x2A;
  const uint8_t COLOR_HEAD     = 0x2F;
  const uint8_t COLOR_FOOD     = 0x4C;
  const uint8_t COLOR_TEXT     = 0x07;

  int sx[MAX_LEN], sy[MAX_LEN];
  int length = 4;
  int dx = 1, dy = 0;

  for (int i = 0; i < length; i++) {
    sx[i] = GX + GW/2 - i;
    sy[i] = GY + GH/2;
  }

  uint32_t rng = 12345;
  auto next_rng = [&]() -> uint32_t {
    rng ^= rng << 13;
    rng ^= rng >> 17;
    rng ^= rng << 5;
    return rng;
  };

  int fx = GX + (next_rng() % (GW - 2)) + 1;
  int fy = GY + (next_rng() % (GH - 2)) + 1;

  int score = 0;
  bool running = true;

  clear_screen_color(COLOR_TEXT);

  move_cursor(0, 0);
  printf("  SNAKE GAME - W/A/S/D ou setas | ENTER=reiniciar  ");

  auto draw_border = [&]() {
    put_char_at(GX - 1,      GY - 1,      '+', COLOR_BORDER);
    put_char_at(GX + GW,     GY - 1,      '+', COLOR_BORDER);
    put_char_at(GX - 1,      GY + GH,     '+', COLOR_BORDER);
    put_char_at(GX + GW,     GY + GH,     '+', COLOR_BORDER);
    for (int x = GX; x < GX + GW; x++) {
      put_char_at(x, GY - 1,  '-', COLOR_BORDER);
      put_char_at(x, GY + GH, '-', COLOR_BORDER);
    }
    for (int y = GY; y < GY + GH; y++) {
      put_char_at(GX - 1,  y, '|', COLOR_BORDER);
      put_char_at(GX + GW, y, '|', COLOR_BORDER);
    }
    for (int y = GY; y < GY + GH; y++)
      for (int x = GX; x < GX + GW; x++)
        put_char_at(x, y, ' ', COLOR_BG);
  };

  draw_border();

  while (running) {
    uint8_t key = read_key_nonblocking();
    if (key == KEY_W || key == KEY_UP)    { if (dy != 1)  { dx = 0;  dy = -1; } }
    if (key == KEY_S || key == KEY_DOWN)  { if (dy != -1) { dx = 0;  dy =  1; } }
    if (key == KEY_A || key == KEY_LEFT)  { if (dx != 1)  { dx = -1; dy =  0; } }
    if (key == KEY_D || key == KEY_RIGHT) { if (dx != -1) { dx =  1; dy =  0; } }
    if (key == ENTER_SCANCODE) {
      reboot_system();
    }

    int nx = sx[0] + dx;
    int ny = sy[0] + dy;

    if (nx < GX || nx >= GX + GW || ny < GY || ny >= GY + GH) {
      running = false;
      break;
    }

    for (int i = 1; i < length; i++) {
      if (nx == sx[i] && ny == sy[i]) {
        running = false;
        break;
      }
    }
    if (!running) break;

    put_char_at(sx[length-1], sy[length-1], ' ', COLOR_BG);

    for (int i = length - 1; i > 0; i--) {
      sx[i] = sx[i-1];
      sy[i] = sy[i-1];
    }

    sx[0] = nx;
    sy[0] = ny;

    if (nx == fx && ny == fy) {
      score++;
      if (length < MAX_LEN) length++;
      do {
        fx = GX + (next_rng() % (GW - 2)) + 1;
        fy = GY + (next_rng() % (GH - 2)) + 1;
      } while (false);
    }

    put_char_at(sx[0], sy[0], 'O', COLOR_HEAD); 
    for (int i = 1; i < length; i++)
      put_char_at(sx[i], sy[i], 'o', COLOR_SNAKE);

    put_char_at(fx, fy, '*', COLOR_FOOD);

    move_cursor(GX + GW + 2, GY);
    printf("Score:");
    if (score == 0) {
      put_char_at(GX + GW + 8, GY, '0', COLOR_TEXT);
    } else {
      char buf[8];
      int tmp = score, idx = 0;
      while (tmp > 0 && idx < 7) { buf[idx++] = '0' + (tmp % 10); tmp /= 10; }
      for (int i = 0; i < idx; i++)
        put_char_at(GX + GW + 8 + (idx - 1 - i), GY, buf[i], COLOR_TEXT);
    }

    //delay(2000000);
    pit_delay_ms(150);
  }

  clear_screen_color(0x40);
  move_cursor(30, 12);
  printf("  GAME OVER!  ");
  move_cursor(25, 13);
  printf("  Pressione ENTER para reiniciar  ");
  while (true) {
    if (read_key_scancode() == ENTER_SCANCODE)
      reboot_system();
  }
}

void TerminalRootOS::pit_delay_ms(uint32_t ms) {
  const uint32_t PIT_FREQ = 1193182;
  const uint16_t PIT_CMD  = 0x43;
  const uint16_t PIT_CH0  = 0x40;

  uint32_t ticks = (PIT_FREQ / 1000) * ms;
  if (ticks > 0xFFFF) ticks = 0xFFFF;

  outb(PIT_CMD, 0x30);
  outb(PIT_CH0, (uint8_t)(ticks & 0xFF));
  outb(PIT_CH0, (uint8_t)((ticks >> 8) & 0xFF));

  while (true) {
    outb(PIT_CMD, 0xE2);
    uint8_t status = inb(PIT_CH0);
    if (status & 0x80) break;
  }
}
