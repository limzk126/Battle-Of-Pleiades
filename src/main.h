//
// Created by dylan on 5/2/22.
//

#ifndef BATTLE_OF_PLEIADES_MAIN_H
#define BATTLE_OF_PLEIADES_MAIN_H

#include "common.h"

extern void initSDL(void);
extern void cleanup(void);
extern void prepareScene(void);
extern void presentScene(void);
extern void doInput(void);
extern void initStage(void);
extern void init_sounds(void);

App app;
Stage stage;

#endif //BATTLE_OF_PLEIADES_MAIN_H
