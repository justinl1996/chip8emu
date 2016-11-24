#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"

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

/*Constructor*/
Chip8::Chip8() 
{
    this->drawFlag = 0;
    this->pc = PROG_START;
    this->stpc = 0;
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
            registers[opcode & 0x0F00] -= registers[(opcode & 0x00F0) >> 4];
            break;
        case 6:
            registers[0xF] = registers[opcode & 0x0F00] & 1;
            registers[opcode & 0x0F00] >>= 1; 
            break;
        case 7:
            if (registers[opcode & 0x00F0] < registers[opcode & 0x0F00]) {
                registers[0xF] = 0;
            } else {
                registers[0xF] = 1;
            }
            registers[opcode & 0x0F00] = registers[opcode & 0x00F0] - 
                    registers[opcode & 0x0F00];
            break;
        case 8:
            registers[0xF] = registers[opcode & 0x0F00] & 0x80;
            registers[0xF] <<= 1;
            break;
    }
}

char Chip8::getKey()
{
    const char *valid = "abcdef1234567890";
    char ch;
    bool flag = false;

    while(1) {
        ch = getchar();
        for (int i = 0; i < (int)strlen(valid); i++) {
            if (valid[i] == ch) {
                flag = true;
                break;
            }
        }
        if (flag) {
            break;   
        }
    }
    return ch;
}

void Chip8::opcodeF()
{
    switch (opcode & 0x00FF) {
        case 0x07:
            registers[(opcode & 0x0F00) >> 8] = delay_timer;
            break; 
        case 0x0A:
            registers[(opcode & 0x0F00) >> 8] = getKey();
            break;
        case 0x15:
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
        case 0x55: {
            unsigned short upto = registers[(opcode & 0x0F00) >> 8];
            for (int i = 0; i < upto; i++, addr_register++) {
                memory[addr_register] = registers[i];
            }
            break;
        }
        case 0x65: {
            unsigned short upto = registers[(opcode & 0x0F00) >> 8];
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

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < 8; col++) {
            pos = 1 << (7 - col);
            state = (memory[addr_register] & pos) >> (7 - col);

            if (screen[x + col][y + row] != state) {
                registers[0xF] = 1;
            }
            screen[x + col][y + row] ^= state ;                     
        }
        addr_register += 1;
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
                    break;
                case 0x00E0:
                    clearScreen();
                    break;               
            }
            break;
        case 0x1000:
            pc = 0x0FFF & opcode;                      
            break;
        case 0x2000:
            stack[stpc++] = pc;
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

