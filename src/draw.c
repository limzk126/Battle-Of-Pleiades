//
// Created by dylan on 5/2/22.
//

#include "draw.h"

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}

SDL_Texture *loadTexture(char *filename) {
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    if (texture == NULL) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
        "Error loading texture with path: %s", filename);
        exit(1);
    }

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
    "Texture with path %s successfully loaded!", filename);

    return texture;
}

void blit(SDL_Texture *texture, int x, int y, double angle) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, flip);
}

void blitRect(SDL_Texture *texture, SDL_Rect src, int x, int y, double angle, int scale_mult) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w * scale_mult;
    dest.h = src.h * scale_mult;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(app.renderer, texture, &src, &dest, angle, NULL, flip);
}
