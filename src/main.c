//
// Created by dylan on 5/2/22.
//

#include "main.h"

/**
 * wait has a value of 16 or 17 depending on the value of remainder (0.667, 1.334 or 1.001).
 * wait has an average value of 16.667 as time goes by.
 * One second has 1000 milliseconds and 60 frames, hence one frame is 16.67 milliseconds.
 * SDL_Delay is used to pad a frame if it takes too little time.
 *
 * @param then The starting tick value.
 * @param remainder The starting remainder value.
 */
static void capFrameRate(long *then , float *remainder) {
    long wait, frameTime;

    wait = 16 + *remainder;

//    printf("total: %ld frames: %ld\n", total, frames);
    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;
//    printf("%ld\n", wait);
    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
    long then = SDL_GetTicks();
    float remainder = 0;

    memset(&app, 0, sizeof(App));
    initSDL();
    atexit(cleanup);

    while(1) {
        prepareScene();
        doInput();
        presentScene();

        capFrameRate(&then, &remainder);
    }

    return 0;
}
#pragma clang diagnostic pop
