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
static void spawnAsteroids(void);
static void drawAsteroids(void);
static void doAsteroids(void);
static void doBullets(void);
static void drawBullets(void);
static void fireBullet(void);
static void do_player_collision();
static void do_bullet_collision();
static void split_asteroid(Entity *parent);
static void drawBackground();
static void do_explosions(void);
static void add_explosions(int x, int y, int num);
static void draw_explosions(void);
static void draw_shield(void);

extern App app;
extern Stage stage;

extern SDL_Texture *loadTexture(char *filename);
extern void blit(SDL_Texture *texture, int x, int y, double angle);
extern void wrapCoordinates(Entity *entity);
extern void blitRect(SDL_Texture *texture, SDL_Rect src, int x, int y, int w, int h, double angle);
extern int is_poly_to_poly_collision(pvector *p1, pvector *p2, int p1_length, int p2_length);
extern void load_music(char *filename);
extern void play_music(int loop);
extern void play_sound(int id, int channel);

#endif //BATTLE_OF_PLEIADES_STAGE_H

