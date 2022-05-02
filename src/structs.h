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

typedef struct {
   float x;
   float y;
   int w;
   int h;
   float dx;
   float dy;
   float angle;
   SDL_Texture *texture;
} Entity;

typedef struct {
    Entity *player;
} Stage;

#endif //BATTLE_OF_PLEIADES_STRUCTS_H
