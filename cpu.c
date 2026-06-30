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

    for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
        cpu->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

}

int loadROM(CPU *cpu, const char *romFilename) {
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
    if (cpu->delayTimer > 0) {
        --cpu->delayTimer;
    }
    if (cpu->soundTimer > 0) {
        --cpu->soundTimer;
    }
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
            memset(cpu->screen, 0, sizeof(cpu->screen));
            break;
        case 0xE:
            cpu->pc = cpu->stack[cpu->sp];
            cpu->sp -= 1;
            break;
        default:
            break;
    }
}
void op_1NNN(CPU *cpu) {
    cpu->pc = cpu->opcode & 0x0FFF;
}
void op_2NNN(CPU *cpu) {
    cpu->sp += 1;
    cpu->stack[cpu->sp] = cpu->pc;
    cpu->pc = cpu->opcode & 0x0FFF;
}
void op_3XKK(CPU *cpu) {
    if (cpu->registers[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF)) {
        cpu->pc += 2;
    }
}
void op_4XKK(CPU *cpu) {
    if (cpu->registers[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF)) {
        cpu->pc += 2;
    }
}
void op_5XY0(CPU *cpu) {
    if (cpu->registers[(cpu->opcode & 0x0F00) >> 8] == cpu->registers[(cpu->opcode & 0x00F0) >> 4]) {
        cpu->pc += 2;
    }
}
void op_6XKK(CPU *cpu) {
    cpu->registers[(cpu->opcode & 0x0F00) >> 8] = cpu->opcode & 0x00FF;
}
void op_7XKK(CPU *cpu) {
    cpu->registers[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
}
void op_8XY(CPU *cpu) {
    const uint8_t x = (cpu->opcode & 0x0F00) >> 8;
    const uint8_t y = (cpu->opcode & 0x00F0) >> 4;
    switch (cpu->opcode & 0x000F) {
        case 0x0:
            cpu->registers[x] = cpu->registers[y];
            break;
        case 0x1:
            cpu->registers[x] = cpu->registers[x] | cpu->registers[y];
            break;
        case 0x2:
            cpu->registers[x] = cpu->registers[x] & cpu->registers[y];
            break;
        case 0x3:
            cpu->registers[x] = cpu->registers[x] ^ cpu->registers[y];
            break;
        case 0x4: {
            uint16_t sum = cpu->registers[x] + cpu->registers[y];
            if (sum > 255) {
                cpu->registers[0xF] = 1;
            }
            sum = sum & 0x00FF;
            cpu->registers[x] = sum;
            break;
        }
        case 0x5: {
            uint8_t difference = cpu->registers[x] - cpu->registers[y];
            if (cpu->registers[x] > cpu->registers[y]) {
                cpu->registers[0xF] = 1;
            } else {
                cpu->registers[0xF] = 0;
            }
            cpu->registers[x] = difference;
            break;
        }
        case 0x6:
            if (cpu->registers[x] & 0x0001) {
                cpu->registers[0xF] = 1;
            } else {
                cpu->registers[0xF] = 0;
            }
            cpu->registers[x] /= 2;
            break;
        case 0x7: {
            uint8_t difference = cpu->registers[y] - cpu->registers[x];
            if (cpu->registers[y] > cpu->registers[x]) {
                cpu->registers[0xF] = 1;
            } else {
                cpu->registers[0xF] = 0;
            }
            cpu->registers[x] = difference;
            break;
        }
        case 0xE:
            if (cpu->registers[x] & 0x80) {
                cpu->registers[0xF] = 1;
            } else {
                cpu->registers[0xF] = 0;
            }
            cpu->registers[x] *= 2;
            break;
        default:
            break;
    }
}
void op_9XY0(CPU *cpu) {
    if (cpu->registers[(cpu->opcode & 0x0F00) >> 8] != cpu->registers[(cpu->opcode & 0x00F0) >> 4]) {
        cpu->pc += 2;
    }
}
void op_ANNN(CPU *cpu) {
    cpu->index = cpu->opcode & 0x0FFF;
}
void op_BNNN(CPU *cpu) {
    cpu->pc = (cpu->opcode & 0x0FFF) + cpu->registers[0];
}
void op_CXKK(CPU *cpu) {
    cpu->registers[(cpu->opcode & 0x0F00) >> 8] = rand() & (cpu->opcode & 0x00FF);
}
void op_DXYN(CPU *cpu) {
    const uint8_t byte_count = cpu->opcode & 0x000F;
    const uint8_t VX = cpu->registers[(cpu->opcode & 0x0F00) >> 8];
    const uint8_t VY = cpu->registers[(cpu->opcode & 0x00F0) >> 4];
    cpu->registers[0xF] = 0;
    for (unsigned int i = cpu->index; i < cpu->index + byte_count; i++) {
        const unsigned int row = i - cpu->index;
        for (unsigned int col = 0; col < 8; col++) {
            uint8_t col_mask = 0x80 >> col;
            uint8_t screenX = (VX + col) % SCREEN_WIDTH;
            uint8_t screenY = (VY + row) % SCREEN_HEIGHT;
            uint32_t *pixel = &cpu->screen[screenY * SCREEN_WIDTH + screenX];
            if (cpu->memory[i] & col_mask) {
                if (*pixel == 0xFFFFFFFF) {
                    cpu->registers[0xF] = 1;
                }
                *pixel ^= 0xFFFFFFFF;
            }
        }
    }
}
void op_EX(CPU *cpu) {
    switch (cpu->opcode & 0x00FF) {
        case 0x9E:
            if (cpu->keys[cpu->registers[(cpu->opcode & 0x0F00) >> 8]] == 1) {
                cpu->pc += 2;
            }
            break;
        case 0xA1:
            if (cpu->keys[cpu->registers[(cpu->opcode & 0x0F00) >> 8]] != 1) {
                cpu->pc += 2;
            }
            break;
        default:
            break;
    }
}
void op_FX(CPU *cpu) {
    switch (cpu->opcode & 0x00FF) {
        case 0x07:
            cpu->registers[(cpu->opcode & 0x0F00) >> 8] = cpu->delayTimer;
            break;
        case 0x0A: {
            unsigned int key_pressed = 0;
            for (unsigned int i = 0; i < 16; i++) {
                if (cpu->keys[i] == 1) {
                    key_pressed = 1;
                    cpu->registers[(cpu->opcode & 0x0F00) >> 8] = i;
                    break;
                }
            }
            if (!key_pressed) {
                cpu->pc -= 2;
            }
            break;
        }
        case 0x15:
            cpu->delayTimer = cpu->registers[(cpu->opcode & 0x0F00) >> 8];
            break;
        case 0x18:
            cpu->soundTimer = cpu->registers[(cpu->opcode & 0x0F00) >> 8];
            break;
        case 0x1E:
            cpu->index = cpu->index + cpu->registers[(cpu->opcode & 0x0F00) >> 8];
            break;
        case 0x29:
            cpu->index = FONTSET_START_ADDRESS + cpu->registers[(cpu->opcode & 0x0F00) >> 8] * CHARACTER_BYTES;
            break;
        case 0x33: {
            unsigned int number = cpu->registers[(cpu->opcode & 0x0F00) >> 8];
            for (int i = 2; i >= 0; i--) {
                const unsigned int digit = number % 10;
                cpu->memory[cpu->index + i] = digit;
                number /= 10;
            }
            break;
        }
        case 0x55: {
            const uint8_t VX = (cpu->opcode & 0x0F00) >> 8;
            for (unsigned int i = 0; i <= VX; i++) {
                cpu->memory[cpu->index + i] = cpu->registers[i];
            }
                break;
        }
        case 0x65: {
            const uint8_t VX = (cpu->opcode & 0x0F00) >> 8;
            for (unsigned int i = 0; i <= VX; i++) {
                cpu->registers[i] = cpu->memory[cpu->index + i];
            }
            break;
        }
        default:
            break;
    }
}