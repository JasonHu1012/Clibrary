#ifndef _COLLISION_H
#define _COLLISION_H

#include "list.h"
#include "vector.h"
#include "ndarray.h"

typedef struct cls_stage cls_stage;
typedef struct cls_object cls_object;

cls_stage *cls_init_stage(
    int dim,
    // border should return negative value when the point falls inside
    // use vec_entry(v, 1 ~ dim) to access vector entry
    double (*border)(vector *v),
    double *lower_bound, // the range of the stage
    double *upper_bound, // make sure the border falls within it
    double max_obj_size // should be as small as possible
);

cls_object *cls_init_object(
    double (*shape)(vector *v), // similar to the border of stage
    double mass,
    // user can kill the vectors afterwards
    vector *position,
    vector *velocity // measured in [length unit]/s
);

void cls_add_detect_point(
    cls_object *obj,
    // user can kill the vectors afterwards
    vector *position, // make sure the point falls on the surface of the object
    vector *normal // normal vector, which should point outwards
);

// user can kill the object afterwardss
void cls_add_object(cls_stage *stage, cls_object *obj);
// interval is measured in micro second
// let round = 0 to run infinately
void cls_start(cls_stage *stage, int interval, int round);
void cls_print(cls_stage *stage);
void cls_kill_stage(cls_stage *stage);
void cls_kill_object(cls_object *obj);

#endif
