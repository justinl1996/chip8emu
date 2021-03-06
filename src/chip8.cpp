#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"
#include "conio.h"
using namespace std;

unsigned char chip8_fontset[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/*Outputs the 16 register in hexadecimal V0 - VF*/
void Chip8::outputRegisters()
{
    for (int i = 0; i < 16; i++) {
        printf("%x:%x  ", i, registers[i]);    
    }
    cout << '\n';
}

/*Return the current program counter*/
unsigned short Chip8::getProgramCounter() 
{
    return pc;
}

/*Return current opcode*/
unsigned short Chip8::getOpCode()
{
    return opcode;
}

unsigned short Chip8::getI()
{
    return addr_register;
}

short Chip8::SDLKeyConvert(SDL_Keycode sdlk)
{
    switch (sdlk) {
        case SDLK_0:
            return 0;
        case SDLK_1:
            return 1;           
        case SDLK_2:
            return 2;
        case SDLK_3:
            return 3;
        case SDLK_4:
            return 4;
        case SDLK_5:
            return 5;
        case SDLK_6:
            return 6;
        case SDLK_7:
            return 7; 
        case SDLK_8:
            return 8;
        case SDLK_9:
            return 9;
        case SDLK_a:
            return 0xA;
        case SDLK_b:
            return 0xB;
        case SDLK_c:
            return 0xC;
        case SDLK_d:
            return 0xD;
        case SDLK_e:
            return 0xE;
        case SDLK_f:
            return 0xF;
        default:
            return -1;
    }
}

/*Constructor*/
Chip8::Chip8() 
{
    this->drawFlag = 0;
    this->pc = PROG_START;
    this->stpc = 0;

    screen = new unsigned char *[S_WIDTH + 10];
    for (int x = 0; x < S_WIDTH + 10; x++) {
        screen[x] = new unsigned char [S_HEIGHT + 10];
    }
    memset(keys, 0, sizeof(short) * 15);
}

Chip8::~Chip8()
{
    for (int x = 0; x < S_WIDTH; x++) {
        delete [] screen[x];
    }
    delete screen;
}

/*Load the font set at the starting at address 0*/
void Chip8::loadFont() 
{
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
}

/*Load a chip 8 rom*/
bool Chip8::loadRom(std::string rom) 
{
    FILE *fp; 
    char ch;
    int i = 0, count = 0;

    if ((fp = fopen(rom.c_str(), "rb")) == NULL) {
        cerr << "error opening file\n";
        return false;
    }

    fseek(fp, 0, SEEK_END);
    count = ftell(fp);

    if (count + PROG_START > MEM_SIZE) {
        cerr << "rom too large\n";
        return false;
    }

    fseek(fp, 0, SEEK_SET);
    
    while (1) {
        ch = fgetc(fp);
        memory[PROG_START + i++] = ch;
        //printf("%d: %x\n", i-1, ch);

        if (i == count) {
            break;
        }
    }
    loadFont();
    return true;
}

/*Clears the screen and redraw*/
void Chip8::clearScreen()
{
    drawFlag = 1;
    for (int w = 0; w < S_WIDTH; w++) {
        for (int h = 0; h < S_HEIGHT; h++) {
            screen[w][h] = 0;
        }
    }
}

/*Execute opcode 0x8***/
void Chip8::opcode8() 
{
    switch(opcode & 0x000F) {
        case 0: 
            registers[(opcode & 0x0F00) >> 8] = 
            registers[(opcode & 0x00F0) >> 4];
            break;
        case 1:
            registers[(opcode & 0x0F00) >> 8] |=  
            registers[(opcode & 0x00F0) >> 4];
            break;
        case 2: 
            registers[(opcode & 0x0F00) >> 8] &= 
            registers[(opcode & 0x00F0) >> 4];
            break;
        case 3:
            registers[(opcode & 0x0F00) >> 8] ^= 
            registers[(opcode & 0x00F0) >> 4];
            break;
        case 4:
            if (registers[(opcode & 0x00F0) >> 4] > (0xFF - 
                        registers[(opcode & 0x0F00) >> 8])) {
                registers[0xF] = 1;
            } else {
                registers[0xF] = 0;   
            }
            registers[(opcode & 0x0F00) >> 8] +=  
            registers[(opcode & 0x00F0) >> 4];
            break;
        case 5:
            if (registers[(opcode & 0x0F00) >> 8] < 
                (registers[(opcode & 0x00F0) >> 4])) {
                registers[0xF] = 0;    
            } else {
                registers[0xF] = 1;
            }
            registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
            break;
        case 6:
            registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 1;
            registers[(opcode & 0x0F00) >> 8] >>= 1; 
            break;
        case 7:
            if (registers[(opcode & 0x00F0) >> 4] < registers[(opcode & 0x0F00) >> 8]) {
                registers[0xF] = 0;
            } else {
                registers[0xF] = 1;
            }
            registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - 
                    registers[(opcode & 0x0F00) >> 8];
            break;
        case 0xE:
            registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x80;
            registers[0xF] <<= 1;
            break;
    }
}

void Chip8::keyInput()
{
    short ch;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                //printf( "Key press detected\n" );
                ch = SDLKeyConvert(event.key.keysym.sym);
                if (ch >= 0) {
                    keys[ch] = 1;
                }
                break;
            case SDL_KEYUP:
                ch = SDLKeyConvert(event.key.keysym.sym);
                if (ch >= 0) {
                    keys[ch] = 0;
                }
                break;
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
            default:
                break;
        }
    }
}

unsigned short Chip8::getKeyBlocking()
{
    short ch = -1;
    while (ch < 0) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    //printf( "Key press detected\n" );
                    ch = SDLKeyConvert(event.key.keysym.sym);
                    break;
                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                default:
                    break;
            }
        }
    }
    return ch;
}


void Chip8::opcodeF()
{
    switch (opcode & 0x00FF) {
        case 0x07:
            registers[(opcode & 0x0F00) >> 8] = delay_timer;
            printf("DELAY USED\n");
            break; 
        case 0x0A:
            registers[(opcode & 0x0F00) >> 8] = getKeyBlocking();
            break;
        case 0x15:
            printf("DELAY SET\n");
            delay_timer = registers[(opcode & 0x0F00) >> 8];
            sound_timer = registers[(opcode & 0x0F00) >> 8];
            break;
        case 0x1E:
            addr_register += registers[(opcode & 0x0F00) >> 8];
            break;   
        case 0x29: {
            unsigned char character = registers[(opcode & 0x0F00) >> 8];
            unsigned short loc;
            if (isdigit(character)) {
                loc = character - '0';        
            } else {
                loc = character - 'A'; 
            }
            addr_register = memory[loc * 5];
            break;
        }
        case 0x33: 
            memory[addr_register] = registers[(opcode & 0x0F00) >> 8] / 100;
            memory[addr_register + 1] = (registers[(opcode & 0x0F00) >> 8] / 
                    10) % 10;
            memory[addr_register + 2] = registers[(opcode & 0x0F00) >> 8] % 10;
            break;
        case 0x55: {
            unsigned short upto = (opcode & 0x0F00) >> 8;
            for (int i = 0; i < upto; i++, addr_register++) {
                memory[addr_register] = registers[i];
            }
            break;
        }
        case 0x65: {
            unsigned short upto = (opcode & 0x0F00) >> 8;
            for (int i = 0; i < upto; i++, addr_register++) {
                registers[i] = memory[addr_register];
            }
            break;   
        }
    }   

}

/*Execute the draw instruction*/
void Chip8::drawSprite()
{
    unsigned short x = registers[(opcode & 0x0F00) >> 8];
    unsigned short y = registers[(opcode & 0x00F0) >> 4];
    unsigned short n = opcode & 0x000F;
    unsigned char pos = 0;
    unsigned char state;
    
    registers[0xF] = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < 8; col++) {
            pos = 1 << (7 - col);
            state = (memory[addr_register + row] & pos) >> (7 - col);

            if (screen[x + col][y + row] != state) {
                registers[0xF] = 1;
            } 
            screen[x + col][y + row] ^= state ;                     
        }
    }
}

/*Execute a single opcode*/
void Chip8::executeCycle()
{
    opcode = (memory[pc] << 8) | (memory[pc+1]);
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode) {
                case 0x00EE:
                    pc = stack[--stpc];  
                    //pc += 2;
                    break;
                case 0x00E0:
                    clearScreen();
                    pc += 2;
                    break;               
            }
            break;
        case 0x1000:
            pc = 0x0FFF & opcode;                      
            break;
        case 0x2000:
            stack[stpc++] = pc + 2;
            pc = 0x0FFF & opcode;
            break;
        case 0x3000:
            if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x4000:
            if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x5000:
            if (registers[(opcode & 0x0F00) >> 8] == 
                        registers[(opcode & 0x00F0) >> 4]) {
                pc += 4;           
            } else {
                pc += 2;
            }
            break;
        case 0x6000:
            registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF); 
            pc += 2;
            break;
        case 0x7000:
            registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);       
            pc += 2;
            break;
        case 0x8000:
            opcode8();
            pc += 2;
            break;           
        case 0x9000:
            if (registers[(opcode & 0x0F00) >> 8] != 
                registers[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0xA000:
            addr_register = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:
            pc = registers[0] + (opcode & 0x0FFF);
            break;
        case 0xC000:
            registers[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);  
            pc += 2;
            break;
        case 0xD000:
            drawSprite();
            pc += 2;
            break;
        case 0xE000:
            if ((opcode & 0x00FF) ==  0x009E) {
                if (keys[registers[(opcode & 0x0F00) >> 8]]) {
                    pc += 4;
                } else {
                    pc += 2;    
                }
            } else if ((opcode & 0x00FF) == 0x00A1) {
                if (!keys[registers[(opcode & 0x0F00) >> 8]]) {
                    pc += 4;
                } else {
                    pc += 2;    
                }               
            }
            break;
        case 0xF000:
            opcodeF();    
        default:
            pc += 2;
            break;
    }
    if (delay_timer > 0) {
        delay_timer--;
    } 
}

