//
// Created by dylan on 5/2/22.
//

#include "utils.h"

void wrapCoordinates(Entity *player, int scale_mult) {

    // Right boundary
    if (player->x > SCREEN_WIDTH) {
        player->x = -player->w;
    }

    // Left boundary
    if (player->x < -player->w * scale_mult) {
        player->x = SCREEN_WIDTH;
    }

    // Top boundary
    if (player->y < -player->h * scale_mult) {
        player->y = SCREEN_HEIGHT;
    }

    // Bottom boundary
    if (player->y > SCREEN_HEIGHT) {
        player->y = -player->h;
    }
}