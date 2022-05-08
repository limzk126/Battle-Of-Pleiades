//
// Created by dylan on 5/2/22.
//

#include "init.h"

void initSDL(void) {
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Battle Of Pleiades", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window) {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
//    putenv("SDL_AUDIODRIVER=alsa");
//    putenv("XDG_RUNTIME_DIR=""");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) {
        printf("Failed to initialize SDL Mixer: %s\n", SDL_GetError());
        exit(1);
    }

    Mix_AllocateChannels(MAX_SND_CHANNELS);
}

void cleanup(void)
{
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    app.renderer = NULL;
    app.window = NULL;

    SDL_Quit();
}
