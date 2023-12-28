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

// kill `json`
// user should only kill the json data returned by `json_parse`
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

// return the length of an array type `json`
int json_arr_size(json_data *json);

// return the `index`-th element of an array type `json`
// user should not kill the returned json data
json_data *json_arr_get(json_data *json, int index);

// return the value of a boolean type `json`
bool json_bool_get(json_data *json);

// return the value of a number type `json`
double json_num_get(json_data *json);

// return the value of a string type `json`
// user should `free` the returned string
char *json_str_get(json_data *json);

#endif
