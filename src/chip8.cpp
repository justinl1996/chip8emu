#include <iostream>
#include <string>
#include <stdio.h>
#include "chip8.h"

using namespace std;

void Chip8::outputRegisters()
{
    for (int i = 0; i < 16; i++) {
        printf("%x ", registers[i]);    
    }
    cout << '\n';
}

unsigned short Chip8::getProgramCounter() 
{
    return pc;
}

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

        if (i == count) {
            break;
        }
        
    }
    return true;
}

