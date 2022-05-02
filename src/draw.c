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

