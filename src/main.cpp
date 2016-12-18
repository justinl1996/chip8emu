#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "gui.h"
#include "chip8.h"

int main(int argc, char **argv)
{
    Chip8 *chip8 = new Chip8();
    chip8->loadRom("INVADERS");
    
    printf("%x\n", 1 - 5);

    /*while (1) {

        std::cout << "PC: "  << chip8->getProgramCounter() << '\n';
        std::cout << "OP: " << std::hex << chip8->getOpCode() << '\n';
        std::cout << "I: " << std::hex << chip8->getI() << '\n';

        chip8->outputRegisters();
        chip8->executeCycle();
        //fgetc(stdin);
    }*/
    VideoOut vo(640, 480);
    if (!vo.init()) {
        std::cerr << "error" << std::endl;
    }

    vo.start();

    delete chip8; 
    
    return 0;
}
