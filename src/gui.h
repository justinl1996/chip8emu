#pragma once
#include <SDL2/SDL.h>

#define S_WIDTH 64
#define S_HEIGHT 48

class VideoOut {
public:
    VideoOut(unsigned _width, unsigned _height);
    ~VideoOut() {
        //SDL_DestroyRenderer(renderer);
        //SDL_DestroyWindow(window);
        for (int i = 0; i < s_height; i++) {
            delete [] screen[i];
        }
        delete [] screen;
        SDL_Quit();
    }
    void start();
    bool init();
    void clearAll();
    void setCoordinate(unsigned int x, unsigned int y) {
        screen[x][y] = 1;
    }
    void clearCoordinate(unsigned int x, unsigned int y) {
        screen[x][y] = 0;
    }
private:
    unsigned int width;
    unsigned int height;
    const unsigned int s_width = S_WIDTH;
    const unsigned int s_height = S_HEIGHT;
    unsigned int **screen;
    //std::vector<int> *screen;
    
    SDL_Surface* surface;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
