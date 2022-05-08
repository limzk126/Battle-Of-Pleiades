//
// Created by dylan on 5/2/22.
//

#include "stage.h"
#include <time.h>

static Entity *player;

static SDL_Texture *playerTexture;
static SDL_Texture *asteroidsTexture;
static SDL_Texture *bulletTexture;
static SDL_Texture *backgroundTexture;
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

    playerTexture = loadTexture("gfx/Pixel_Spaceships/Sprites/blue_03.png");
    asteroidsTexture = loadTexture("gfx/asteroids-arcade.png");
    bulletTexture = loadTexture("gfx/Pixel_Spaceships/Sprites/Projectiles/projectile02-1-cropped.png");
    backgroundTexture = loadTexture("gfx/background.jpg");

    initRects();
    initPlayer();

    load_music("music/Mercury.ogg");
    play_music(1);

    srand(time(NULL));
}

static void logic(void) {
    doPlayer();
    spawnAsteroids();
    doAsteroids();
    doBullets();
    do_player_collision();
    do_bullet_collision();
}

static void draw(void) {
    drawBackground();
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
    player->texture = playerTexture;
    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = 0;
    rect->y = 0;
    SDL_QueryTexture(playerTexture, NULL, NULL, &rect->w, &rect->h);
    player->rect = rect;
    player->w = player->rect->w * 7 / 6;
    player->h = player->rect->h * 7 / 6;
    player->health = 1;
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
            play_sound(SND_PLAYER_FIRE, CH_PLAYER);
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

static void drawBackground() {
    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = 0;
    rect->y = 0;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &rect->w, &rect->h);
    blitRect(backgroundTexture, *rect, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, rect->w, rect->h, 0);
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
            if (y / x * SCREEN_WIDTH <= SCREEN_HEIGHT) {
                x = SCREEN_WIDTH;
                y = y / x * SCREEN_WIDTH;
            } else {
                y = SCREEN_HEIGHT;
                x = x / y * SCREEN_HEIGHT;
            }
            x = SCREEN_WIDTH / 2 + sin(theta) * 900;
            y = SCREEN_WIDTH / 2 - cos(theta) * 900;
            Entity *asteroid;
            asteroid = malloc(sizeof(Entity));
            memset(asteroid, 0, sizeof(Entity));

            stage.asteroidTail->next = asteroid;
            stage.asteroidTail = asteroid;

            asteroid->x = x;
            asteroid->y = y;
            asteroid->angle = 360 * ((double) rand()) / RAND_MAX;
            asteroid->dx = 2 * sin(asteroid->angle * UNIT_DEGREE_IN_RADIANS);
            asteroid->dy = -2 * cos(asteroid->angle * UNIT_DEGREE_IN_RADIANS);
            asteroid->texture = asteroidsTexture;
            asteroid->rect = &texture_portion_rect[large_asteroid];
            asteroid->w = asteroid->rect->w * 2;
            asteroid->h = asteroid->rect->h * 2;
            asteroid->health = 1;
            asteroid->split_count = 0;
        }
    }
}

static void split_asteroid(Entity *parent) {
    if (parent->split_count >= 2) {
        printf("true\n");
        return;
    }

    for (int i = 0; i < 2; ++i) {
        Entity *child;
        child = malloc(sizeof(Entity));
        memset(child, 0, sizeof(Entity));

        stage.asteroidTail->next = child;
        stage.asteroidTail = child;

        child->x = parent->x;
        child->y = parent->y;
        child->angle = 360 * ((double) rand()) / RAND_MAX;
        child->dx = 3 * sin(child->angle * UNIT_DEGREE_IN_RADIANS);
        child->dy = -3 * cos(child->angle * UNIT_DEGREE_IN_RADIANS);
        child->texture = asteroidsTexture;
        child->rect = &texture_portion_rect[large_asteroid];
        child->w = parent->w / 2;
        child->h = parent->h / 2;
        child->health = 1;
        child->split_count = parent->split_count + 1;
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
    Entity *e;

    Entity *prev = &stage.asteroidHead;
    for (e = stage.asteroidHead.next; e != NULL; e = e->next) {
        if (e->health == 0) {
            prev->next = e->next;

            if (e == stage.asteroidTail) {
                stage.asteroidTail = prev;
            }

            free(e);
            e = prev;
        }

        prev = e;

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
//    printf("%d %d\n", player->rect->x, player->rect->y);
//    float px = player->x, py = player->y;
//    float bx = player->x + texture_portion_rect[player_bullet].w / 2, by = player->y + texture_portion_rect[player_bullet].h / 2;
//    bullet->x = px + (bx - px) * cos(rad) - (by - py) * sin(rad);
//    bullet->y = py + (bx - px) * sin(rad) + (by - py) * cos(rad);
//    bullet->angle = player->angle;
//    bullet->dx = 5 * sin(player->angle * UNIT_DEGREE_IN_RADIANS);
//    bullet->dy = 5 * -cos(player->angle * UNIT_DEGREE_IN_RADIANS);
//    bullet->texture = asteroidsTexture;
//    bullet->rect = &texture_portion_rect[player_bullet];
//    bullet->w = bullet->rect->w;
//    bullet->h = bullet->rect->h;
    bullet->x = player->x;
    bullet->y = player->y;
    bullet->angle = player->angle;
    bullet->dx = 14 * sin(rad);
    bullet->dy = 14 * - cos(rad);
    bullet->texture = bulletTexture;
    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = 0;
    rect->y = 0;
    SDL_QueryTexture(bulletTexture, NULL, NULL, &rect->w, &rect->h);
    bullet->rect = rect;
    bullet->w = bullet->rect->w;
    bullet->h = bullet->rect->h;
    bullet->health = 1;
}

static void drawBullets(void) {
    Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next) {
        blitRect(b->texture, *(b->rect), b->x, b->y, b->w, b->h, b->angle);
    }
}

static void doBullets(void) {
    Entity * e;

    Entity *prev = &stage.bulletHead;
    for (e = stage.bulletHead.next; e != NULL; e = e->next) {
        if (e->health == 0) {
            prev->next = e->next;

            if (e == stage.bulletTail) {
                stage.bulletTail = prev;
            }

            free(e);
            e = prev;
        }

        prev = e;

        e->x += e->dx;
        e->y += e->dy;
    }
}

int get_new_x_after_rotation(int current_x,int w, int h, float rotation_angle) {
    return current_x + w * cos(rotation_angle) - h * sin(rotation_angle);
}

int get_new_y_after_rotation(int current_y, int w, int h, float rotation_angle) {
    return current_y + w / 2 * sin(rotation_angle) + h / 2 * cos(rotation_angle);
}

pvector get_new_vertex_after_rotation(int current_x, int current_y, int w, int h, float rotation_angle) {
    pvector new_vertex = {
        .x = current_x + w * cos(rotation_angle) - h * sin(rotation_angle),
        .y = current_y + w * sin(rotation_angle) + h * cos(rotation_angle)
    };

    return new_vertex;
}
void generate_vertices(pvector *arr, Entity e, int is_player) {
    if (is_player) {
        const pvector top_middle = get_new_vertex_after_rotation(e.x, e.y, 0, -(e.h / 2), e.angle * UNIT_DEGREE_IN_RADIANS);
        const pvector bottom_left = get_new_vertex_after_rotation(e.x, e.y, -(e.w / 2), (e.h / 2), e.angle * UNIT_DEGREE_IN_RADIANS);
        const pvector bottom_right = get_new_vertex_after_rotation(e.x, e.y, e.w / 2, (e.h / 2), e.angle * UNIT_DEGREE_IN_RADIANS);

        arr[0] = top_middle;
        arr[1] = bottom_right;
        arr[2] = bottom_left;

        return;
    }

    const pvector top_left = get_new_vertex_after_rotation(e.x, e.y, -(e.w / 2), -(e.h / 2), e.angle);
    const pvector top_right = get_new_vertex_after_rotation(e.x, e.y, (e.w / 2), -(e.h / 2), e.angle);
    const pvector bottom_left = get_new_vertex_after_rotation(e.x, e.y, -(e.w / 2), (e.h / 2), e.angle);
    const pvector bottom_right = get_new_vertex_after_rotation(e.x, e.y, (e.w / 2), (e.h / 2), e.angle);

    arr[0] = top_left;
    arr[1] = top_right;
    arr[2] = bottom_right;
    arr[3] = bottom_left;
}

static void do_player_collision() {
    Entity *a;

    pvector player_vertices[3];
    pvector * p = player_vertices;
    generate_vertices(p, *stage.player, 1);

    int next = 0;
    for (int i = 0; i < 3; ++i) {
        next = i + 1;
        if (i == 2) next = 0;
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(app.renderer, player_vertices[i].x, player_vertices[i].y, player_vertices[next].x, player_vertices[next].y);
    }
    for (a = stage.asteroidHead.next; a != NULL; a = a->next) {
        pvector asteroid_vertices[4];
        pvector * as = asteroid_vertices;
        generate_vertices(as, *a, 0);
        for (int i = 0; i < 4; ++i) {
            next = i + 1;
            if (i == 3) next = 0;
            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(app.renderer, asteroid_vertices[i].x, asteroid_vertices[i].y, asteroid_vertices[next].x, asteroid_vertices[next].y);
        }
        if (is_poly_to_poly_collision(p, as, 3, 4)) {
            player->x = SCREEN_WIDTH / 2;
            player->y = SCREEN_HEIGHT / 2;
            play_sound(SND_PLAYER_DIE, CH_PLAYER);
            return;
        }
    }
}

static void do_bullet_collision() {
    Entity *b;
    pvector bullet_vertices[4];
    pvector *p = bullet_vertices;
    for (b = stage.bulletHead.next; b != NULL; b = b->next) {
        generate_vertices(p, *b, 0);

        Entity *a;
        pvector asteroid_vertices[4];
        pvector *as = asteroid_vertices;
        for (a = stage.asteroidHead.next; a != NULL; a = a->next) {
            generate_vertices(as, *a, 0);
            if (is_poly_to_poly_collision(p, as, 4, 4) && a->health) {
                b->health = 0;
                a->health = 0;
                split_asteroid(a);
                break;
            }
        }
    }
}

