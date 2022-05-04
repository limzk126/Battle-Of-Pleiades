//
// Created by dylan on 5/2/22.
//

#include "utils.h"

void wrapCoordinates(Entity *entity) {
    // Right boundary
    if (entity->x > SCREEN_WIDTH) {
        entity->x = -entity->w;
    }

    // Left boundary
    if (entity->x < -entity->w) {
        entity->x = SCREEN_WIDTH;
    }

    // Top boundary
    if (entity->y < -entity->h) {
        entity->y = SCREEN_HEIGHT;
    }

    // Bottom boundary
    if (entity->y > SCREEN_HEIGHT) {
        entity->y = -entity->h;
    }
}