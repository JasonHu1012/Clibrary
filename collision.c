#include "collision.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct detect_point detect_point;

struct cls_stage {
    int dim;
    double block_size;
    int *block_num;
    list *obj_list;
    double (*border)(vector *);
    double *base;
    ndarray *block;
};

struct cls_object {
    double (*shape)(vector *v);
    list *detect_list;
    double mass;
    vector *position;
    vector *velocity;
    bool in_block;
};

struct detect_point {
    vector *position;
    vector *normal;
};

cls_stage *cls_init_stage(int dim, double (*border)(vector *),
    double *lower_bound, double *upper_bound, double max_obj_size) {
    assert(dim > 0);
    cls_stage *output = (cls_stage *)malloc(sizeof(cls_stage));
    output->dim = dim;
    output->block_size = max_obj_size;
    output->block_num = (int *)malloc(sizeof(int) * dim);
    for (int i = 0; i < dim; i++) {
        assert(upper_bound[i] >= lower_bound[i]);
        output->block_num[i] = (upper_bound[i] - lower_bound[i]) / max_obj_size + 1;
    }
    output->obj_list = lst_init(sizeof(list_node *));
    output->border = border;
    output->base = (double *)malloc(sizeof(double) * dim);
    memcpy(output->base, lower_bound, sizeof(double) * dim);
    output->block = nda_init(output->block_num, dim, sizeof(list *));
    return output;
}

cls_object *cls_init_object(double (*shape)(vector *), double mass,
    vector *position, vector *velocity) {
    assert(mass > 0);
    cls_object *output = (cls_object *)malloc(sizeof(cls_object));
    output->shape = shape;
    output->detect_list = lst_init(sizeof(detect_point *));
    output->mass = mass;
    output->position = vec_copy(position);
    output->velocity = vec_copy(velocity);
    output->in_block = false;
    return output;
}

static detect_point *init_detect_point(vector *position, vector *normal) {
    detect_point *output = (detect_point *)malloc(sizeof(detect_point));
    output->position = vec_copy(position);
    output->normal = vec_copy(normal);
    return output;
}

void cls_add_detect_point(cls_object *obj, vector *position, vector *normal) {
    detect_point *dp = init_detect_point(position, normal);
    lst_add_tail(obj->detect_list, &dp, NULL);
}

static cls_object *object_copy(cls_object *obj) {
    cls_object *output = cls_init_object(obj->shape, obj->mass, obj->position, obj->velocity);
    list_iter *it = lst_init_iter(obj->detect_list, false);
    detect_point *dp;
    while (lst_iter_next(it, &dp)) {
        detect_point *clone_dp = init_detect_point(dp->position, dp->normal);
        lst_add_tail(output->detect_list, &clone_dp, NULL);
    }
    return output;
}

void cls_add_object(cls_stage *stage, cls_object *obj) {
    cls_object *clone_obj = object_copy(obj);
    clone_obj->in_block = true;
    list_node *n = lst_init_node(&clone_obj, sizeof(cls_object *));
    lst_add_tail(stage->obj_list, &n, NULL);
}

void cls_start(cls_stage *stage, int interval, int round) {
}

void cls_print(cls_stage *stage) {
    list_iter *it = lst_init_iter(stage->obj_list, false);
    list_node *n;
    cls_object *obj;
    int count = 1;
    while (lst_iter_next(it, &n)) {
        lst_get_node(n, &obj, sizeof(cls_object));
        printf("---object %d---\n", count++);
        printf("mass: %f\n", obj->mass);
        printf("position: ");
        vec_print(obj->position);
        printf("velocity: ");
        vec_print(obj->velocity);
    }
    lst_kill_iter(it);
}

void cls_kill_stage(cls_stage *stage) {
}

void cls_kill_object(cls_object *obj) {
}
