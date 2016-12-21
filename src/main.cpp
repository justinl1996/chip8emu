#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <SDL2/SDL.h>
#include "gui.h"
#include "chip8.h"

int main(int argc, char **argv)
{
    Chip8 *chip8 = new Chip8();
    chip8->loadRom("../games/INVADERS");
    
    printf("%x\n", 1 - 5);
    VideoOut vo(640, 480);
    if (!vo.init()) {
        std::cerr << "error" << std::endl;
    }
    bool playing = true;

    while (playing) {

        //std::cout << "PC: "  << chip8->getProgramCounter() << '\n';
        //std::cout << "OP: " << std::hex << chip8->getOpCode() << '\n';
        //std::cout << "I: " << std::hex << chip8->getI() << '\n';
        //chip8->outputRegisters();

        chip8->executeCycle();
        chip8->keyInput();
        //fgetc(stdin);
        if (vo.draw(chip8->getScreen())) {
            break; 
        }
        usleep(500);
        //getchar()
        /*SDL_Event event;
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        std::cout << "left key pressed\n";
                    }
                    printf( "Key press detected\n" );
                    break;

                case SDL_KEYUP:
                    printf( "Key release detected\n" );
                    break;
                
                case SDL_QUIT:
                    playing = false;
                default:
                    break;
            }
         }*/
    }

    //delete chip8; 
    
    return 0;
}
