#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void cpuInit(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    cpu->index = 0;
    cpu->pc = START_ADDRESS;
    memset(cpu->stack, 0, sizeof(cpu->stack));
    cpu->sp = 0;
    cpu->soundTimer = 0;
    cpu->delayTimer = 0;
    memset(cpu->keys, 0, sizeof(cpu->keys));
    memset(cpu->screen, 0, sizeof(cpu->screen));
    cpu->opcode = 0;

    const unsigned int FONTSET_SIZE = 80;
    uint8_t fontset[FONTSET_SIZE] = {
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

    const unsigned int FONTSET_START_ADDRESS = 0x50;

    for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
        cpu->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

}

int loadROM(CPU *cpu, char *romFilename) {
    FILE *romFile = fopen(romFilename, "rb");
    if (romFile == NULL) {
        perror(romFilename);
        return ERROR_READING_ROM;
    }
    if (fseek(romFile, 0, SEEK_END) != 0) {
        fclose(romFile);
        return ERROR_READING_ROM;
    }
    long size = ftell(romFile);
    if (size == -1) {
        fclose(romFile);
        return ERROR_READING_ROM;
    }
    if (fseek(romFile, 0, SEEK_SET) != 0) {
        fclose(romFile);
        return ERROR_READING_ROM;
    }
    if (size > MAX_ROM_SIZE) {
        fclose(romFile);
        return ERROR_READING_ROM;
    }
    uint8_t buffer[size];
    if (fread(buffer, 1, size, romFile) != size) {
        fclose(romFile);
        return ERROR_READING_ROM;
    }
    fclose(romFile);
    for (long i = 0; i < size; ++i) {
        cpu->memory[START_ADDRESS + i] = buffer[i];
    }
    return SUCCESS;
}

void cpuCycle(CPU *cpu) {
    cpu->opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    uint8_t opcode_id = cpu->opcode >> 12;
    op_table[opcode_id](cpu);
}

void (*op_table[0x10])(CPU *cpu) = {
    op_00E,
    op_1NNN,
    op_2NNN,
    op_3XKK,
    op_4XKK,
    op_5XY0,
    op_6XKK,
    op_7XKK,
    op_8XY,
    op_9XY0,
    op_ANNN,
    op_BNNN,
    op_CXKK,
    op_DXYN,
    op_EX,
    op_FX
};

void op_00E(CPU *cpu) {
    switch (cpu->opcode & 0x000F) {
        case 0x0:
            break;
        case 0xE:
            break;
        default:
            break;
    }
}
void op_1NNN(CPU *cpu) {}
void op_2NNN(CPU *cpu) {}
void op_3XKK(CPU *cpu) {}
void op_4XKK(CPU *cpu) {}
void op_5XY0(CPU *cpu) {}
void op_6XKK(CPU *cpu) {}
void op_7XKK(CPU *cpu) {}
void op_8XY(CPU *cpu) {
    switch (cpu->opcode & 0x000F) {
        case 0x0:
            break;
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
            break;
        case 0x4:
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
            break;
        case 0xE:
            break;
        default:
            break;
    }
}
void op_9XY0(CPU *cpu) {}
void op_ANNN(CPU *cpu) {}
void op_BNNN(CPU *cpu) {}
void op_CXKK(CPU *cpu) {}
void op_DXYN(CPU *cpu) {}
void op_EX(CPU *cpu) {
    switch (cpu->opcode & 0x00FF) {
        case 0xA1:
            break;
        case 0x9E:
            break;
        default:
            break;
    }
}
void op_FX(CPU *cpu) {
    switch (cpu->opcode & 0x00FF) {
        case 0x07:
            break;
        case 0x0A:
            break;
        case 0x18:
            break;
        case 0x1E:
            break;
        case 0x29:
            break;
        case 0x33:
            break;
        case 0x55:
            break;
        case 0x65:
            break;
        default:
            break;
    }
}