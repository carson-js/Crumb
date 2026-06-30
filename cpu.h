#pragma once
#include <stdlib.h>
#include <stdint.h>

#define START_ADDRESS 0x200
#define ERROR_READING_ROM 2
#define SUCCESS 0
#define MAX_ROM_SIZE 0xE00
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define FONTSET_START_ADDRESS  0x50
#define FONTSET_SIZE 80
#define CHARACTER_BYTES 5

typedef struct {
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t soundTimer;
    uint8_t delayTimer;
    uint8_t keys[16];
    uint32_t screen[64 * 32];
    uint16_t opcode;
} CPU;

void cpuInit(CPU *cpu);

void (*op_table[0x10])(CPU *cpu);
void op_00E(CPU *cpu);
void op_1NNN(CPU *cpu);
void op_2NNN(CPU *cpu);
void op_3XKK(CPU *cpu);
void op_4XKK(CPU *cpu);
void op_5XY0(CPU *cpu);
void op_6XKK(CPU *cpu);
void op_7XKK(CPU *cpu);
void op_8XY(CPU *cpu);
void op_9XY0(CPU *cpu);
void op_ANNN(CPU *cpu);
void op_BNNN(CPU *cpu);
void op_CXKK(CPU *cpu);
void op_DXYN(CPU *cpu);
void op_EX(CPU *cpu);
void op_FX(CPU *cpu);