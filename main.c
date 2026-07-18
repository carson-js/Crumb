#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "cpu.h"
#include "platform.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s <Scale> <Delay> <ROM>\n", argv[0]);
        exit(1);
    }
    int scale = atoi(argv[1]);
    int delay = atoi(argv[2]);
    CPU cpu;
    Platform platform;
    srand(time(NULL));
    cpuInit(&cpu);
    if (platformInit(&platform, scale)) {
        printf("Error initializing SDL2 platform\n");
        return 1;
    }
    if (loadROM(&cpu, argv[3]) != 0) {
        perror("Error loading ROM\n");
        return 2;
    }
    int quit = 0;
    while (!quit) {
        quit = platformProcessInput(&cpu);
        SDL_Delay(delay);
        cpuCycle(&cpu);
        platformRender(&platform, &cpu);
    }
	platformDestroy(&platform);
    return 0;
}
