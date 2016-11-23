#include <iostream>
#include <string>
#include <stdio.h>
#include "chip8.h"

using namespace std;

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
        printf("%d: %x\n", i-1, ch);

        if (i == count) {
            break;
        }
        
    }
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
        default:
            pc += 2;
            break;
    }

}

