//
// Created by dylan on 5/2/22.
//

#include "utils.h"

void wrapCoordinates(Entity *player) {

    // Right boundary
    if (player->x > SCREEN_WIDTH) {
        player->x = -player->w;
    }

    // Left boundary
    if (player->x < -player->w) {
        player->x = SCREEN_WIDTH;
    }

    // Top boundary
    if (player->y < -player->h) {
        player->y = SCREEN_HEIGHT;
    }

    // Bottom boundary
    if (player->y > SCREEN_HEIGHT) {
        player->y = -player->h;
    }
}