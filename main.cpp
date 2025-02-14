#include "kernel.hpp"

extern "C" void kmain(void* multiboot_structure, unsigned int magicnumber){

  TerminalRootOS terminal;

  terminal.printf("  +-----------------------------------------------+\n");
  terminal.printf("  |           ::Terminal Root OS::                |\n");
  terminal.printf("  +-----------------------------------------------+\n");
  terminal.printf("  |         https://terminalroot.com.br/          |\n");
  terminal.printf("  +-----------------------------------------------+\n");
  terminal.printf("  |      PRESSIONE [ENTER] PARA REINICIAR         |\n");
  terminal.printf("  +-----------------------------------------------+\n");

  while (1){
    uint8_t scancode = terminal.read_key_scancode();
    if (scancode == terminal.ENTER_SCANCODE) {
      terminal.printf("\nReiniciando o sistema...\n");
      terminal.reboot_system(); // Reiniciar o sistema
    }
  }
}

