#include <iostream>

#include "chip8.h"

int main(int argc, char **argv)
{
    Chip8 *chip8 = new Chip8;
        
    chip8->loadRom("games/INVADERS"); 


    delete chip8; 
    
    return 0;
}
