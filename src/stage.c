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

    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void doPlayer(void) {

}

static void initLL(void) {

}

static void drawPlayer(void) {
    blit(stage.player->texture, stage.player->x, stage.player->y);
}

