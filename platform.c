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

void platformRender(Platform *platform, CPU *cpu) {
    SDL_UpdateTexture(platform->texture, NULL, cpu->screen, 64 * sizeof(uint32_t));
	SDL_RenderCopy(platform->renderer, platform->texture, NULL, NULL);
    SDL_RenderPresent(platform->renderer);
}

int platformProcessInput(CPU *cpu) {
    int quit = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                    case SDLK_x:
                        cpu->keys[0] = 1;
                        break;
                    case SDLK_1:
                        cpu->keys[1] = 1;
                        break;
                    case SDLK_2:
                        cpu->keys[2] = 1;
                        break;
                    case SDLK_3:
                        cpu->keys[3] = 1;
                        break;
                    case SDLK_q:
                        cpu->keys[4] = 1;
                        break;
                    case SDLK_w:
                        cpu->keys[5] = 1;
                        break;
                    case SDLK_e:
                        cpu->keys[6] = 1;
                        break;
                    case SDLK_a:
                        cpu->keys[7] = 1;
                        break;
                    case SDLK_s:
                        cpu->keys[8] = 1;
                        break;
                    case SDLK_d:
                        cpu->keys[9] = 1;
                        break;
                    case SDLK_z:
                        cpu->keys[0xA] = 1;
                        break;
                    case SDLK_c:
                        cpu->keys[0xB] = 1;
                        break;
                    case SDLK_4:
                        cpu->keys[0xC] = 1;
                        break;
                    case SDLK_r:
                        cpu->keys[0xD] = 1;
                        break;
                    case SDLK_f:
                        cpu->keys[0xE] = 1;
                        break;
                    case SDLK_v:
                        cpu->keys[0xF] = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                        quit = 1;
                        break;
                case SDLK_x:
                        cpu->keys[0] = 0;
                        break;
                case SDLK_1:
                        cpu->keys[1] = 0;
                        break;
                case SDLK_2:
                        cpu->keys[2] = 0;
                        break;
                case SDLK_3:
                        cpu->keys[3] = 0;
                        break;
                case SDLK_q:
                        cpu->keys[4] = 0;
                        break;
                case SDLK_w:
                        cpu->keys[5] = 0;
                        break;
                case SDLK_e:
                        cpu->keys[6] = 0;
                        break;
                case SDLK_a:
                        cpu->keys[7] = 0;
                        break;
                case SDLK_s:
                        cpu->keys[8] = 0;
                        break;
                case SDLK_d:
                        cpu->keys[9] = 0;
                        break;
                case SDLK_z:
                        cpu->keys[0xA] = 0;
                        break;
                case SDLK_c:
                        cpu->keys[0xB] = 0;
                        break;
                case SDLK_4:
                        cpu->keys[0xC] = 0;
                        break;
                case SDLK_r:
                        cpu->keys[0xD] = 0;
                        break;
                case SDLK_f:
                        cpu->keys[0xE] = 0;
                        break;
                case SDLK_v:
                        cpu->keys[0xF] = 0;
                        break;
                }
                break;
        }
    }
    return quit;
}