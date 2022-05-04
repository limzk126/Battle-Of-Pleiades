//
// Created by dylan on 5/2/22.
//

#include "stage.h"
#include <time.h>

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
    player_bullet,
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

    texture_portion_rect[player_bullet].x = 0;
    texture_portion_rect[player_bullet].y = textureH / 2;
    texture_portion_rect[player_bullet].w = textureW / 4;
    texture_portion_rect[player_bullet].h = textureH / 4;
}

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.asteroidTail = &stage.asteroidHead;
    stage.bulletTail = &stage.bulletHead;

    playerTexture = loadTexture("gfx/fighter.png");
    asteroidsTexture = loadTexture("gfx/asteroids-arcade.png");

    initRects();
    initPlayer();

    srand(time(NULL));
}

static void logic(void) {
    doPlayer();
    spawnAsteroids();
    doAsteroids();
    doBullets();
}

static void draw(void) {
    drawPlayer();
    drawAsteroids();
    drawBullets();
}

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.player = player;
    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;
    player->texture = asteroidsTexture;
    player->angle = 0;
    player->rect = &texture_portion_rect[brown_spaceship];
    player->w = player->rect->w * 2;
    player->h = player->rect->h * 2;
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
    if (app.keyboard[SDL_SCANCODE_LCTRL]) {
        if (app.keyboard[SDL_SCANCODE_LCTRL]++ == 1) {
            fireBullet();
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP]) {
//        printf("%f  %f\n",player->dx, player->dy);
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
    wrapCoordinates(stage.player);
    blitRect(stage.player->texture, *stage.player->rect,stage.player->x,
            stage.player->y, stage.player->w,stage.player->h, stage.player->angle);
}

static void spawnAsteroids() {
    if (stage.asteroidHead.next == NULL) {
        for (int i = 0; i < 5; ++i) {
            double r = 400 * sqrt(((double) rand()) / RAND_MAX);
            double theta =  ((double) rand()) / RAND_MAX * 2 * M_PI;
            double x = SCREEN_WIDTH / 2 + r + cos(theta);
            double y = SCREEN_HEIGHT / 2 + r + sin(theta);
            printf("BEFORE x = %f, y = %f\n", x, y);
            if (y / x * SCREEN_WIDTH <= SCREEN_HEIGHT) {
                x = SCREEN_WIDTH;
                y = y / x * SCREEN_WIDTH;
            } else {
                y = SCREEN_HEIGHT;
                x = x / y * SCREEN_HEIGHT;
            }
            x = SCREEN_WIDTH / 2 + sin(theta) * 900;
            y = SCREEN_WIDTH / 2 - cos(theta) * 900;
            printf("x = %f, y = %f\n", x, y);
            Entity *asteroid;
            asteroid = malloc(sizeof(Entity));
            memset(asteroid, 0, sizeof(Entity));

            stage.asteroidTail->next = asteroid;
            stage.asteroidTail = asteroid;

            asteroid->x = x;
            asteroid->y = y;
            asteroid->angle = 360 * ((double) rand()) / RAND_MAX;
            asteroid->dx = 5 * sin(asteroid->angle * UNIT_DEGREE_IN_RADIANS);
            asteroid->dy = -5 * cos(asteroid->angle * UNIT_DEGREE_IN_RADIANS);
            asteroid->texture = asteroidsTexture;
            asteroid->rect = &texture_portion_rect[large_asteroid];
            asteroid->w = asteroid->rect->w * 2;
            asteroid->h = asteroid->rect->h * 2;
        }
    }
}

static void drawAsteroids(void) {
    Entity *e;

    for (e = stage.asteroidHead.next; e != NULL; e = e->next) {
        wrapCoordinates(e);
        blitRect(e->texture, *(e->rect), e->x, e->y, e->w, e->h, e->angle);
    }
}

static void doAsteroids(void) {
    Entity * e;

    for (e = stage.asteroidHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;
    }
}

static void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    double rad = player->angle * UNIT_DEGREE_IN_RADIANS;
    printf("%d %d\n", player->rect->x, player->rect->y);
    float px = player->x, py = player->y;
    float bx = player->x + texture_portion_rect[player_bullet].w / 2, by = player->y + texture_portion_rect[player_bullet].h / 2;
    bullet->x = px + (bx - px) * cos(rad) - (by - py) * sin(rad);
    bullet->y = py + (bx - px) * sin(rad) + (by - py) * cos(rad);
    bullet->angle = player->angle;
    bullet->dx = 5 * sin(player->angle * UNIT_DEGREE_IN_RADIANS);
    bullet->dy = 5 * -cos(player->angle * UNIT_DEGREE_IN_RADIANS);
    bullet->texture = asteroidsTexture;
    bullet->rect = &texture_portion_rect[player_bullet];
    bullet->w = bullet->rect->w;
    bullet->h = bullet->rect->h;
}

static void drawBullets(void) {
    Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next) {
        blitRect(b->texture, *(b->rect), b->x, b->y, b->w, b->h, b->angle);
    }
}

static void doBullets(void) {
    Entity * e;

    for (e = stage.bulletHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;
    }
}
