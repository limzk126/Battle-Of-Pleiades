//
// Created by dylan on 5/2/22.
//

#include "stage.h"

static Entity *player;

static SDL_Texture *playerTexture;
static SDL_Texture *asteroidsTexture;
static SDL_Rect texture_portion_rect[64];

enum texturePortion {
    brown_spaceship,
    strong_thrust_one,
    weak_thrust_one,
    strong_thrust_two,
    weak_thrust_two,
    large_asteroid,
    medium_asteroid_tl,
    medium_asteroid_tr,
    medium_asteroid_bl,
    medium_asteroid_br,
};

void initRects() {
    int textureW;
    int textureH;

    SDL_QueryTexture(asteroidsTexture, NULL, NULL, &textureW, &textureH);

    texture_portion_rect[brown_spaceship].x = 0;
    texture_portion_rect[brown_spaceship].y = 0;
    texture_portion_rect[brown_spaceship].w = textureW / 8;
    texture_portion_rect[brown_spaceship].h = textureH / 8;

    texture_portion_rect[strong_thrust_one].x = textureW / 4;
    texture_portion_rect[strong_thrust_one].y = 0;
    texture_portion_rect[strong_thrust_one].w = textureW / 8;
    texture_portion_rect[strong_thrust_one].h = textureH / 8;

    texture_portion_rect[weak_thrust_one].x = textureW * 3 / 8;
    texture_portion_rect[weak_thrust_one].y = 0;
    texture_portion_rect[weak_thrust_one].w = textureW / 8;
    texture_portion_rect[weak_thrust_one].h = textureH / 8;

    texture_portion_rect[strong_thrust_two].x = textureW / 2;
    texture_portion_rect[strong_thrust_two].y = 0;
    texture_portion_rect[strong_thrust_two].w = textureW / 8;
    texture_portion_rect[strong_thrust_two].h = textureH / 8;

    texture_portion_rect[weak_thrust_two].x = textureW * 5 / 8;
    texture_portion_rect[weak_thrust_two].y = 0;
    texture_portion_rect[weak_thrust_two].w = textureW / 8;
    texture_portion_rect[weak_thrust_two].h = textureH / 8;

    texture_portion_rect[large_asteroid].x = textureW / 4;
    texture_portion_rect[large_asteroid].y = textureH * 3 / 4;
    texture_portion_rect[large_asteroid].w = textureW / 4;
    texture_portion_rect[large_asteroid].h = textureH / 4;

    texture_portion_rect[medium_asteroid_tl].x = textureW / 2;
    texture_portion_rect[medium_asteroid_tl].y = textureH * 3 / 4;
    texture_portion_rect[medium_asteroid_tl].w = textureW / 8;
    texture_portion_rect[medium_asteroid_tl].h = textureH / 8;

    texture_portion_rect[medium_asteroid_tr].x = textureW * 5 / 8;
    texture_portion_rect[medium_asteroid_tr].y = textureH * 3 / 4;
    texture_portion_rect[medium_asteroid_tr].w = textureW / 8;
    texture_portion_rect[medium_asteroid_tr].h = textureH / 8;

    texture_portion_rect[medium_asteroid_bl].x = textureW / 2;
    texture_portion_rect[medium_asteroid_bl].y = textureH * 7 / 8;
    texture_portion_rect[medium_asteroid_bl].w = textureW / 8;
    texture_portion_rect[medium_asteroid_bl].h = textureH / 8;

    texture_portion_rect[medium_asteroid_br].x = textureW * 5 / 8;
    texture_portion_rect[medium_asteroid_br].y = textureH * 7 / 8;
    texture_portion_rect[medium_asteroid_br].w = textureW / 8;
    texture_portion_rect[medium_asteroid_br].h = textureH / 8;

}

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    initLL();

    playerTexture = loadTexture("gfx/fighter.png");
    asteroidsTexture = loadTexture("gfx/asteroids-arcade.png");

    initRects();
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

    wrapCoordinates(player);
}

static void initLL(void) {

}

static void drawPlayer(void) {
    blitRect(asteroidsTexture, texture_portion_rect[brown_spaceship],stage.player->x,
            stage.player->y, stage.player->angle, 2);
}

