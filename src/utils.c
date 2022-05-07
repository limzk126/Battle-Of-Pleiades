//
// Created by dylan on 5/2/22.
//

#include "utils.h"

static int is_line_to_poly_intercept(pvector *vertices, int num_vertices, float x1, float y1, float x2, float y2);
static int is_line_to_line_intercept(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

void wrapCoordinates(Entity *entity) {
    // Right boundary
    if (entity->x > SCREEN_WIDTH) {
        entity->x = -entity->w;
    }

    // Left boundary
    if (entity->x < -entity->w) {
        entity->x = SCREEN_WIDTH;
    }

    // Top boundary
    if (entity->y < -entity->h) {
        entity->y = SCREEN_HEIGHT;
    }

    // Bottom boundary
    if (entity->y > SCREEN_HEIGHT) {
        entity->y = -entity->h;
    }
}

int is_poly_to_poly_collision(pvector *p1, pvector *p2, int p1_length, int p2_length) {
    int next = 0;
    for (int current = 0; current < p1_length; ++current) {
        next = current + 1;
        if (next == p1_length) {
            next = 0;
        }

        pvector vc = p1[current];
        pvector vn = p1[next];

        int is_collision = is_line_to_poly_intercept(p2, p2_length, vc.x, vc.y, vn.x, vn.y);
        if (is_collision) {
            return 1;
        }
    }

    return 0;
}

static int is_line_to_poly_intercept(pvector *vertices, int num_vertices, float x1, float y1, float x2, float y2) {

    int next = 0;
    for (int current = 0; current < num_vertices; ++current) {
        next = current + 1;
        if (next == num_vertices) {
            next = 0;
        }

        float x3 = vertices[current].x;
        float y3 = vertices[current].y;
        float x4 = vertices[next].x;
        float y4 = vertices[next].y;

        int is_intercept = is_line_to_line_intercept(x1, y1, x2, y2, x3, y3, x4, y4);
        if (is_intercept) {
            return 1;
        }
    }

    return 0;
}

static int is_line_to_line_intercept(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

    // calculate the direction of the lines
    float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

    if (uA >= 0 && uA <= 1 && uB >=0 && uB <= 1) {
        return 1;
    }

    return 0;
}