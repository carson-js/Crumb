#include <stdio.h>
#include "cpu.h"
#include "platform.h"

int platformInit(Platform *platform, const int scale) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Quit();
        return 1;
    }
    platform->window = SDL_CreateWindow(
        "Crumb",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale,
        SDL_WINDOW_SHOWN
    );
    if (platform->window == NULL) {
        SDL_Quit();
        return 1;
    }
    platform->renderer = SDL_CreateRenderer(platform->window, -1, SDL_RENDERER_ACCELERATED);
    if (platform->renderer == NULL) {
        SDL_DestroyWindow(platform->window);
        SDL_Quit();
        return 1;
    }
    platform->texture = SDL_CreateTexture(
        platform->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    if (platform->texture == NULL) {
        SDL_DestroyWindow(platform->window);
        SDL_DestroyRenderer(platform->renderer);
        SDL_Quit();
        return 1;
    }
    return 0;
}

void platformDestroy(Platform *platform) {
    SDL_DestroyWindow(platform->window);
    SDL_DestroyRenderer(platform->renderer);
    SDL_DestroyTexture(platform->texture);
    SDL_Quit();
}