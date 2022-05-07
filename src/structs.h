//
// Created by dylan on 5/2/22.
//

#ifndef BATTLE_OF_PLEIADES_STRUCTS_H
#define BATTLE_OF_PLEIADES_STRUCTS_H

typedef struct {
    void (*logic)(void);
    void (*draw)(void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity {
   float x;
   float y;
   int w;
   int h;
   int scale_mult;
   float dx;
   float dy;
   double angle;
   SDL_Texture *texture;
   SDL_Rect *rect;
   struct Entity *next;
} Entity;

typedef struct {
    Entity *player;
    Entity asteroidHead, *asteroidTail;
    Entity bulletHead, *bulletTail;
} Stage;

typedef struct {
    int x;
    int y;
} pvector;

#endif //BATTLE_OF_PLEIADES_STRUCTS_H
