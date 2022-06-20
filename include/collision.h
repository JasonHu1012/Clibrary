#ifndef _COLLISION_H
#define _COLLISION_H

#include "list.h"
#include "vector.h"
#include "ndarray.h"

typedef struct room room;
typedef struct ball ball;

room *cls_init_room( // room is a square
    int dim,
    // the range of the room
    double *range_min,
    double *range_max,
    double max_ball_radius // should be as small as possible for better performance
);
ball *cls_init_ball(
    double radius,
    double mass,
    // user can kill the vectors afterwards
    vector *position,
    vector *velocity // measured in [length unit]/s
);
// user can kill the ball afterwardss
void cls_add_ball(room *r, ball *b);
void cls_print(room *r);
void cls_start(
    room *r,
    int interval, // measured in micro second,
                  // should be small enough to avoid a ball going
                  // through other balls or out of the room
    int round // let round < 0 to run infinitely
);
void cls_kill_room(room *r);
void cls_kill_ball(ball *b);

#endif
