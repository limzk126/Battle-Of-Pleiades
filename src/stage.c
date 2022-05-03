//
// Created by dylan on 5/2/22.
//

#include "stage.h"

static Entity *player;
static SDL_Texture *playerTexture;

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    initLL();

    playerTexture = loadTexture("gfx/fighter.png");

    initPlayer();
}

static void logic(void) {
    doPlayer();
}

static void draw(void) {
    drawPlayer();
}

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.player = player;
    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;
    player->texture = playerTexture;
    player->angle = 0;

    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static int isOverSpeedLimit(void) {
    float dx = player->dx;
    float dy = player->dy;

    float currentSpeed = sqrt(dx * dx + dy * dy);

    return currentSpeed > PLAYER_MAX_SPEED;
}

static void throttleSpeed(float *dx, float *dy) {
    float currentSpeed = sqrt(*dx * *dx + *dy * *dy);

    *dx *= PLAYER_MAX_SPEED / currentSpeed;
    *dy *= PLAYER_MAX_SPEED / currentSpeed;
}

static void doPlayer(void) {
    if (app.keyboard[SDL_SCANCODE_LEFT]) {
        player->angle -= 4.5;
        if (player->angle < -360) {
            player->angle += 360;
        }

        if (player->angle > 360) {
            player->angle -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->angle += 4.5;
        if (player->angle < -360) {
            player->angle += 360;
        }

        if (player->angle > 360) {
            player->angle -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP]) {
        printf("%f  %f\n",player->dx, player->dy);
        player->dx += sin(player->angle * UNIT_DEGREE_IN_RADIANS) * DURATION_PER_FRAME * ACCELERATION_MULT;
        player->dy -= cos(player->angle * UNIT_DEGREE_IN_RADIANS) * DURATION_PER_FRAME * ACCELERATION_MULT;
    }

    if (!app.keyboard[SDL_SCANCODE_UP]) {
        player->dx = player->dx - player->dx * DURATION_PER_FRAME * DRAG_MULT;
        player->dy = player->dy - player->dy * DURATION_PER_FRAME * DRAG_MULT;
    }

    if (isOverSpeedLimit()) {
        throttleSpeed(&player->dx, &player->dy);
    }

    player->x += player->dx;
    player->y += player->dy;


}

static void initLL(void) {

}

static void drawPlayer(void) {
    blit(stage.player->texture, stage.player->x, stage.player->y, stage.player->angle);
}

