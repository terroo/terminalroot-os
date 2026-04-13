#include "kernel.hpp"

extern "C" void kmain(void* multiboot_structure, unsigned int magicnumber){
  TerminalRootOS terminal;

  terminal.printf(" +-----------------------------------------------+\n");
  terminal.printf(" | ::Terminal Root OS::                          |\n");
  terminal.printf(" +-----------------------------------------------+\n");
  terminal.printf(" | https://terminalroot.com.br/                  |\n");
  terminal.printf(" +-----------------------------------------------+\n");
  terminal.printf(" | [ENTER] REINICIAR  |  [P] SNAKE GAME          |\n");
  terminal.printf(" +-----------------------------------------------+\n");

  while (1) {
    uint8_t scancode = terminal.read_key_scancode();

    if (scancode == terminal.ENTER_SCANCODE) {
      terminal.printf("\nReiniciando o sistema...\n");
      terminal.reboot_system();
    }

    if (scancode == terminal.KEY_P) {
      terminal.run_snake();
      // run_snake só retorna em game over + ENTER → já chama reboot
      // mas caso retorne, voltamos ao menu:
      terminal.printf("\nVoltando ao menu...\n");
    }
  }
}
