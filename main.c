#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s <Scale> <Delay> <ROM>\n", argv[0]);
        exit(1);
    }
    int scale = atoi(argv[1]);
    int delay = atoi(argv[2]);
    CPU cpu;
    srand(time(NULL));
    cpuInit(&cpu);
    if (loadROM(&cpu, argv[3]) != 0) {
        perror("Error loading ROM\n");
        return 2;
    }


    return 0;
}
