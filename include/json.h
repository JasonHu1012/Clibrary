#ifndef _JSON_H
#define _JSON_H

#include <stdbool.h>

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_NULL
} JSON_TYPE;

typedef struct json_data json_data;

// return parsed result of `json_str`
json_data *json_parse(char *json_str);

// initialize an empty json object
json_data *json_obj_init();

// initialize an empty json array
json_data *json_arr_init();

// initialize a json boolean with value `value`
json_data *json_bool_init(bool value);

// initialize a json number with value `value`
json_data *json_num_init(double value);

// initailize a json string with value `value`
// `value` will be copied
json_data *json_str_init(char *value);

// initialize a json null
json_data *json_null_init();

// kill `json`
// user should only kill the outermost json structure
void json_kill(json_data *json);

// return whether `json_str` is in valid format
bool json_is_valid(char *json_str);

// transform `json` to string and user should `free` it
// `sort` decides whether to sort (asc) the keys of object
char *json_to_str(json_data *json, bool sort);

// return the type of `json`
JSON_TYPE json_type(json_data *json);

// return the size of an object type `json`
int json_obj_size(json_data *json);

// return the keys of an object type `json`
// the length of the array can be retrieved by `json_obj_size`
// user should `free` the array and the strings
char **json_obj_keys(json_data *json);

// return `json[key]` where `json` is object type
// return NULL when key isn't found
// user should not kill the returned json data
json_data *json_obj_get(json_data *json, char *key);

// let `json[key]` be `value` where `json` is object type
// note that `value` won't be copied
// if `json[key]` already exists, the old one will be killed
void json_obj_set(json_data *json, char *key, json_data *value);

// remove `key` from object type `json`
// `key` must be in `json`
// `json[key]` will be killed
void json_obj_remove(json_data *json, char *key);

// return the length of an array type `json`
int json_arr_size(json_data *json);

// return the `json[index]` where `json` is array type
// user should not kill the returned json data
json_data *json_arr_get(json_data *json, int index);

// let `json[index]` be `value` where `json` is array type
// note that `value` won't be copied, and the old value will be killed
void json_arr_set(json_data *json, int index, json_data *value);

// append `value` to array type `json`
// note that `value` won't be copied
void json_arr_append(json_data *json, json_data *value);

// remove `json[index]` where `json` is array type
// `json[index]` will be killed
void json_arr_remove(json_data *json, int index);

// return the value of a boolean type `json`
bool json_bool_get(json_data *json);

// return the value of a number type `json`
double json_num_get(json_data *json);

// return the value of a string type `json`
// user should `free` the returned string
char *json_str_get(json_data *json);

#endif
