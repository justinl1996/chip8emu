#include <iostream>
#define MEM_SIZE 4096
#define PROG_START 0x200


class Chip8 {
public:
    bool loadRom(std::string);
    void executeCycle();
    void outputRegisters();
    unsigned short getProgramCounter();

private:
    unsigned short pc;
    unsigned short stpc;
    unsigned short stack[16];
    unsigned short registers[16];
    unsigned char memory[MEM_SIZE];
    unsigned char screen[64][48];
};

