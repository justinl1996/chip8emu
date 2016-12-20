#include <iostream>
#define MEM_SIZE 4096
#define PROG_START 0x200
#define S_WIDTH 64
#define S_HEIGHT 48

class Chip8 {
public:
    Chip8();
    ~Chip8();
    bool loadRom(std::string);
    void executeCycle();
    void outputRegisters();
    unsigned short getProgramCounter();
    unsigned short getOpCode();
    unsigned short getI();
    bool drawState() {
        return drawFlag;
    };
    unsigned char **getScreen() {
        return screen;   
    };

private:
    void clearScreen();
    void opcode8();
    void opcodeF();
    void drawSprite();
    void loadFont();
    char getKey();

    unsigned short pc;
    unsigned short stpc;
    unsigned short stack[16];
    unsigned char registers[16];
    unsigned short addr_register;
    unsigned short opcode; 
    unsigned char memory[MEM_SIZE];
    unsigned char **screen;
    unsigned short delay_timer;
    unsigned short sound_timer;

    bool drawFlag;
};

