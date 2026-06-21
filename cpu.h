#pragma once
#include <stdlib.h>
#include <stdint.h>

#define START_ADDRESS 0x200
#define ERROR_READING_ROM 2
#define SUCCESS 0
#define MAX_ROM_SIZE 0xE00

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