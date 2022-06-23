#ifndef _COLLISION_H
#define _COLLISION_H

#include "list.h"
#include "vector.h"
#include "ndarray.h"
#include <stdbool.h>

typedef struct room room;
typedef struct ball ball;

room *cls_init_room( // room is a square
    int dim,
    // the range of the room, in meter
    double *range_min,
    double *range_max,
    double max_ball_radius // should be as small as possible for better performance
);
ball *cls_init_ball(
    double radius,
    double mass, // kilogram
    // user can kill the vectors afterwards
    vector *position,
    vector *velocity // meters per second
);
// user can kill the ball afterwardss
void cls_add_ball(room *r, ball *b);
void cls_print(room *r);
double cls_kinetic(room *r); // Joule
void cls_start(
    room *r,
    int interval, // the logical time between two frames,
                  // measured in microsecond,
                  // should be small enough to avoid a ball going
                  // through other balls or out of the room,
                  // must be larger than zero
    int rounds // let round < 0 to run infinitely
);
// use SDL to show the process, only 2d
void cls_start_show(
    room *r,
    int interval, // the logical time between two frames,
                  // measured in microsecond,
                  // should be small enough to avoid a ball going
                  // through other balls or out of the room,
                  // must be larger than zero
    int rounds, // let round < 0 to run infinitely
    int fps,
    bool fill // whether to fill the ball
);
void cls_kill_room(room *r);
void cls_kill_ball(ball *b);

#endif
