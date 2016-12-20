#include <iostream>
#include <vector>
#include <thread>
#include "gui.h"

VideoOut::VideoOut(unsigned _width, unsigned _height) : window(NULL), 
        surface(NULL), renderer(NULL), width(_width), height(_height) 
{
    /*screen = new unsigned int *[s_width];
    for (int i = 0; i < s_width; i++) {
        screen[i] = new unsigned int[s_height]; 
    }

    clearAll();*/
}

/*void VideoOut::clearAll()
{
    for (int x = 0; x < s_width; x++) {
        for (int y = 0; y < s_height; y++) {
            screen[x][y] = 0;   
        }
    }
}*/

bool VideoOut::init()
{
    auto disp_err = []() {
        std::cerr << "SDL error: " << SDL_GetError() << std::endl;        
    };
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        disp_err();
        return false;
    }

    window = SDL_CreateWindow("CHIP 8 EMULATOR", SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN); 

    if (window == NULL) {
        disp_err();
        return false;
    } else {
        surface = SDL_GetWindowSurface(window);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
    if (renderer == NULL) {
        disp_err();   
        return false;
    } else {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  return true;
}

bool VideoOut::draw(unsigned char **screen) 
{
    SDL_Event e;
    bool quit = false;
    unsigned short block_width = width/s_width;
    unsigned short block_height = height/s_height;

    while (SDL_PollEvent( &e ) != 0 ) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);

    for (int x = 0; x < s_width; x++) {
        for (int y = 0; y < s_height; y++) {
            if (screen[x][y] == 1) {             
                SDL_Rect fillRect = {x * block_width, y * block_height, block_width, 
                        block_height};
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
    return quit;
}

