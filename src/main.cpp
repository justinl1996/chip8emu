#include <iostream>
#include <stdio.h>

#include "chip8.h"

int main(int argc, char **argv)
{
    Chip8 *chip8 = new Chip8();
    chip8->loadRom("MAZE");
    
    printf("%x\n", 1 - 5);
     
    while (1) {

        std::cout << "PC: "  << chip8->getProgramCounter() << '\n';
        std::cout << "OP: " << std::hex << chip8->getOpCode() << '\n';

        chip8->outputRegisters();
        chip8->executeCycle();
        fgetc(stdin);
    }



    delete chip8; 
    
    return 0;
}
