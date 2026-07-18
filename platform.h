#pragma once
#include <SDL2/SDL.h>
#include "cpu.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Platform;

int platformInit(Platform *platform, int scale);
void platformDestroy(Platform *platform);
void platformRender(Platform *platform, CPU *cpu);
int platformProcessInput(CPU *cpu);