//
// Created by dylan on 5/2/22.
//

#ifndef BATTLE_OF_PLEIADES_STAGE_H
#define BATTLE_OF_PLEIADES_STAGE_H

#include "common.h"

static void logic(void);
static void draw(void);
static void doPlayer(void);
static void initLL(void);
static void initPlayer(void);
static void drawPlayer(void);
static int isOverSpeedLimit(void);
static void throttleSpeed(float* dx, float *dy);

extern App app;
extern Stage stage;

extern SDL_Texture *loadTexture(char *filename);
extern void blit(SDL_Texture *texture, int x, int y, double angle);
extern void wrapCoordinates(Entity *player);
extern void blitRect(SDL_Texture *texture, SDL_Rect src, int x, int y, double angle, int scale_mult);

#endif //BATTLE_OF_PLEIADES_STAGE_H

