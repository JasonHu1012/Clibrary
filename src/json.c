#include "json.h"
#include "table.h"
#include "list.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INDENT 2

typedef struct obj_data {
    table *data;
    // used to store the original order of keys
    list *keys;
} obj_data;

struct json_data {
    JSON_TYPE type;
    // the type of `data` is:
    // object: (obj_data *)
    // array: (list *)
    // boolean: (bool *)
    // number: (double *)
    // string: (char *)
    void *data;
};

static json_data *parse_general(char *json_str, int *i);
static bool general_is_valid(char *json_str, int *i, bool *is_empty);
static int general_strlen(json_data *json, int level);
static void general_to_str(json_data *json, int level, bool sort, char *ret, int *ret_i);

static bool is_ignored_char(char c) {
    if (c == ' ' ||
        c == '\r' ||
        c == '\n' ||
        c == '\t') {
        return true;
    }
    return false;
}

static int str_cmp(void const *a, void const *b) {
    return strcmp(*(char **)a, *(char **)b);
}

// return parsed result of `json_str`
json_data *json_parse(char *json_str) {
    assert(json_is_valid(json_str));

    int i = 0;
    return parse_general(json_str, &i);
}

void json_kill(json_data *json) {
    switch (json->type) {
        case JSON_OBJECT:
        {
            table *tbl = ((obj_data *)json->data)->data;
            list *keys = ((obj_data *)json->data)->keys;
            int size = lst_size(keys);

            for (int i = 0; i < size; i++) {
                char *key;
                lst_get(keys, i, &key);

                json_data *sub_json;
                tbl_get(tbl, key, &sub_json);

                json_kill(sub_json);
                free(key);
            }

            tbl_kill(tbl);
            lst_kill(keys);
            free(json->data);

            break;
        }
        
        case JSON_ARRAY:
        {
            list *lst = (list *)json->data;
            int size = lst_size(lst);

            for (int i = 0; i < size; i++) {
                json_data *sub_json;
                lst_get(lst, i, &sub_json);

                json_kill(sub_json);
            }

            lst_kill(lst);

            break;
        }

        case JSON_BOOLEAN:
        case JSON_NUMBER:
        case JSON_STRING:
        {
            free(json->data);
            break;
        }

        default:
        {
            break;
        }
    }

    free(json);
}

bool json_is_valid(char *json_str) {
    int i = 0;
    if (!general_is_valid(json_str, &i, NULL)) {
        return false;
    }

    // check the remaining part
    for (i++; json_str[i]; i++) {
        if (!is_ignored_char(json_str[i])) {
            return false;
        }
    }
    return true;
}

char *json_to_str(json_data *json, bool sort) {
    int len = general_strlen(json, 0);
    char *ret = (char *)malloc(sizeof(char) * (len + 1));
    int ret_i = 0;
    general_to_str(json, 0, sort, ret, &ret_i);
    ret[len] = 0;
    return ret;
}

JSON_TYPE json_type(json_data *json) {
    return json->type;
}

int json_obj_size(json_data *json) {
    assert(json->type == JSON_OBJECT);

    table *tbl = ((obj_data *)json->data)->data;

    return tbl_size(tbl);
}

char **json_obj_keys(json_data *json) {
    assert(json->type == JSON_OBJECT);

    table *tbl = ((obj_data *)json->data)->data;

    return tbl_keys(tbl);
}

json_data *json_obj_get(json_data *json, char *key) {
    assert(json->type == JSON_OBJECT);

    table *tbl = ((obj_data *)json->data)->data;

    json_data *ret = NULL;
    tbl_get(tbl, key, &ret);
    return ret;
}

int json_arr_size(json_data *json) {
    assert(json->type == JSON_ARRAY);

    list *lst = (list *)json->data;

    return lst_size(lst);
}

json_data *json_arr_get(json_data *json, int index) {
    assert(json->type == JSON_ARRAY);

    list *lst = (list *)json->data;

    assert(index >= 0 && index < lst_size(lst));

    json_data *ret;
    lst_get(lst, index, &ret);
    return ret;
}

bool json_bool_get(json_data *json) {
    assert(json->type == JSON_BOOLEAN);

    return *(bool *)json->data;
}

double json_num_get(json_data *json) {
    assert(json->type == JSON_NUMBER);

    return *(double *)json->data;
}

char *json_str_get(json_data *json) {
    assert(json->type == JSON_STRING);

    char *ret = (char *)malloc(sizeof(char) * (strlen((char *)json->data) + 1));
    strcpy(ret, (char *)json->data);
    return ret;
}

static char *encode_string(char *s) {
    int ret_len = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '\\' ||
            s[i] == '\n' ||
            s[i] == '\r' ||
            s[i] == '\t' ||
            s[i] == '\b' ||
            s[i] == '\f' ||
            s[i] == '"') {
            ret_len++;
        }
        ret_len++;
    }

    char *ret = (char *)malloc(sizeof(char) * (ret_len + 1));
    ret_len = 0;
    for (int i = 0; s[i]; i++) {
        switch (s[i]) {
            case '\\':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = '\\';
                break;
            }
            case '\n':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = 'n';
                break;
            }
            case '\r':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = 'r';
                break;
            }
            case '\t':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = 't';
                break;
            }
            case '\b':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = 'b';
                break;
            }
            case '\f':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = 'f';
                break;
            }
            case '"':
            {
                ret[ret_len++] = '\\';
                ret[ret_len++] = '"';
                break;
            }
            default:
            {
                ret[ret_len++] = s[i];
            }
        }
    }
    ret[ret_len++] = 0;
    return ret;
}

static void obj_to_str(json_data *json, int level, bool sort, char *ret, int *ret_i) {
    table *tbl = ((obj_data *)json->data)->data;
    list *keys_ = ((obj_data *)json->data)->keys;

    int size = lst_size(keys_);
    char **keys = (char **)malloc(sizeof(char *) * size);
    for (int i = 0; i < size; i++) {
        // copy strings in list to array, don't free the strings
        lst_get(keys_, i, &keys[i]);
    }

    if (sort) {
        qsort(keys, size, sizeof(char *), str_cmp);
    }

    // "{\n"
    sprintf(ret + *ret_i, "{\n");
    *ret_i += 2;

    for (int i = 0; i < size; i++) {
        json_data *sub_json;
        tbl_get(tbl, keys[i], &sub_json);

        // indent before key
        memset(ret + *ret_i, ' ', sizeof(char) * (level + 1) * INDENT);
        *ret_i += (level + 1) * INDENT;

        // key
        char *encoded = encode_string(keys[i]);
        sprintf(ret + *ret_i, "\"%s\"", encoded);
        *ret_i += 2 + strlen(encoded);
        free(encoded);

        // ": "
        sprintf(ret + *ret_i, ": ");
        *ret_i += 2;

        // value
        general_to_str(sub_json, level + 1, sort, ret, ret_i);

        if (i < size - 1) {
            // ","
            ret[(*ret_i)++] = ',';
        }
        // "\n"
        ret[(*ret_i)++] = '\n';
    }

    // indent before "}"
    memset(ret + *ret_i, ' ', sizeof(char) * level * INDENT);
    *ret_i += level * INDENT;

    // "}"
    ret[(*ret_i)++] = '}';

    free(keys);
}

static void arr_to_str(json_data *json, int level, bool sort, char *ret, int *ret_i) {
    list *lst = (list *)json->data;

    int size = lst_size(lst);

    // "[\n"
    sprintf(ret + *ret_i, "[\n");
    *ret_i += 2;

    for (int i = 0; i < size; i++) {
        json_data *sub_json;
        lst_get(lst, i, &sub_json);

        // indent before sub json
        memset(ret + *ret_i, ' ', sizeof(char) * (level + 1) * INDENT);
        *ret_i += (level + 1) * INDENT;

        // sub json
        general_to_str(sub_json, level + 1, sort, ret, ret_i);

        if (i < size - 1) {
            // ","
            ret[(*ret_i)++] = ',';
        }
        // "\n"
        ret[(*ret_i)++] = '\n';
    }

    // indent before "]"
    memset(ret + *ret_i, ' ', sizeof(char) * level * INDENT);
    *ret_i += level * INDENT;

    // "]"
    ret[(*ret_i)++] = ']';
}

static void bool_to_str(json_data *json, char *ret, int *ret_i) {
    if (*(bool *)json->data) {
        sprintf(ret + *ret_i, "true");
        *ret_i += 4;
    }
    else {
        sprintf(ret + *ret_i, "false");
        *ret_i += 5;
    }
}

static void num_to_str(json_data *json, char *ret, int *ret_i) {
    double dnum = *(double *)json->data;
    long long lnum = (long long)dnum;

    if (dnum == (double)lnum) {
        sprintf(ret + *ret_i, "%lld", lnum);
    }
    else {
        sprintf(ret + *ret_i, "%f", dnum);
    }

    *ret_i += strlen(ret + *ret_i);
}

static void str_to_str(json_data *json, char *ret, int *ret_i) {
    char *s = (char *)json->data;
    char *encoded = encode_string(s);
    sprintf(ret + *ret_i, "\"%s\"", encoded);
    *ret_i += strlen(ret + *ret_i);
    free(encoded);
}

static void null_to_str(json_data *json, char *ret, int *ret_i) {
    sprintf(ret + *ret_i, "null");
    *ret_i += 4;
}

static void general_to_str(json_data *json, int level, bool sort, char *ret, int *ret_i) {
    switch (json->type) {
        case JSON_OBJECT:
        {
            obj_to_str(json, level, sort, ret, ret_i);
            break;
        }
        case JSON_ARRAY:
        {
            arr_to_str(json, level, sort, ret, ret_i);
            break;
        }
        case JSON_BOOLEAN:
        {
            bool_to_str(json, ret, ret_i);
            break;
        }
        case JSON_NUMBER:
        {
            num_to_str(json, ret, ret_i);
            break;
        }
        case JSON_STRING:
        {
            str_to_str(json, ret, ret_i);
            break;
        }
        case JSON_NULL:
        {
            null_to_str(json, ret, ret_i);
            break;
        }
        default:
        {
            break;
        }
    }
}

static int obj_strlen(json_data *json, int level) {
    table *tbl = ((obj_data *)json->data)->data;

    int size = tbl_size(tbl);
    char **keys = tbl_keys(tbl);

    // "{\n"
    int ret = 2;
    for (int i = 0; i < size; i++) {
        json_data *sub_json;
        tbl_get(tbl, keys[i], &sub_json);

        // indent before key
        ret += (level + 1) * INDENT;
        // key
        char *encoded = encode_string(keys[i]);
        ret += 2 + strlen(encoded);
        free(encoded);
        // ": "
        ret += 2;
        // value
        ret += general_strlen(sub_json, level + 1);
        // "\n" or ",\n"
        ret += i == size - 1 ? 1 : 2;

        free(keys[i]);
    }
    // indent and "}"
    ret += level * INDENT + 1;

    free(keys);

    return ret;
}

static int arr_strlen(json_data *json, int level) {
    list *lst = (list *)json->data;

    int size = lst_size(lst);

    // "[\n"
    int ret = 2;
    for (int i = 0; i < size; i++) {
        json_data *sub_json;
        lst_get(lst, i, &sub_json);

        // indent before sub json
        ret += (level + 1) * INDENT;
        // sub json
        ret += general_strlen(sub_json, level + 1);
        // "\n" or ",\n"
        ret += i == size - 1 ? 1 : 2;
    }
    // indent and "]"
    ret += level * INDENT + 1;
    return ret;
}

static int bool_strlen(json_data *json) {
    return *(bool *)json->data ? 4 : 5;
}

static int num_strlen(json_data *json) {
    char s[32];
    double dnum = *(double *)json->data;
    long long lnum = (long long)dnum;

    if (dnum == (double)lnum) {
        sprintf(s, "%lld", lnum);
    }
    else {
        sprintf(s, "%f", dnum);
    }

    return strlen(s);
}

static int str_strlen(json_data *json) {
    char *s = (char *)json->data;
    int ret = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '\\' ||
            s[i] == '\n' ||
            s[i] == '\r' ||
            s[i] == '\t' ||
            s[i] == '\b' ||
            s[i] == '\f' ||
            s[i] == '"') {
            ret++;
        }
        ret++;
    }
    return ret + 2;
}

static int null_strlen(json_data *json) {
    return 4;
}

static int general_strlen(json_data *json, int level) {
    switch (json->type) {
        case JSON_OBJECT:
        {
            return obj_strlen(json, level);
        }
        case JSON_ARRAY:
        {
            return arr_strlen(json, level);
        }
        case JSON_BOOLEAN:
        {
            return bool_strlen(json);
        }
        case JSON_NUMBER:
        {
            return num_strlen(json);
        }
        case JSON_STRING:
        {
            return str_strlen(json);
        }
        case JSON_NULL:
        {
            return null_strlen(json);
        }
        default:
        {
            break;
        }
    }

    return -1;
}

static int transit_general_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == '{') {
                return 1;
            }
            if (c == '[') {
                return 2;
            }
            if (c == 't' || c == 'f') {
                return 3;
            }
            if (c == '-' || c == '.' ||
                (c >= '0' && c <= '9')) {
                return 4;
            }
            if (c == '"') {
                return 5;
            }
            if (c == 'n') {
                return 6;
            }
            return 7;
        }
        case 7:
        {
            // unknown character
            return -1;
        }
        default:
        {
            // invalid, not used
            return -1;
        }
    }
}

static int transit_obj_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == '{') {
                return 1;
            }
            return -1;
        }
        case 1:
        {
            // expected to parse key or empty
            return -1;
        }
        case 2:
        {
            // expected to parse key
            return -1;
        }
        case 3:
        {
            // finish parsing key
            if (is_ignored_char(c)) {
                return 3;
            }
            if (c == ':') {
                return 4;
            }
            return -1;
        }
        case 4:
        {
            // expected to parse value
            return -1;
        }
        case 5:
        {
            // finish parsing value
            if (is_ignored_char(c)) {
                return 5;
            }
            if (c == ',') {
                return 2;
            }
            if (c == '}') {
                return 7;
            }
            return -1;
        }
        case 6:
        {
            // expected to end
            if (is_ignored_char(c)) {
                return 6;
            }
            if (c == '}') {
                return 7;
            }
            return -1;
        }
        case 7:
        {
            // end
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

static int transit_arr_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == '[') {
                return 1;
            }
            return -1;
        }
        case 1:
        {
            // expected to parse a new json or empty
            return -1;
        }
        case 2:
        {
            // expected to parse a new json
            return -1;
        }
        case 3:
        {
            // finish parsing a new json
            if (is_ignored_char(c)) {
                return 3;
            }
            if (c == ',') {
                return 2;
            }
            if (c == ']') {
                return 5;
            }
            return -1;
        }
        case 4:
        {
            // expected to end
            if (is_ignored_char(c)) {
                return 4;
            }
            if (c == ']') {
                return 5;
            }
            return -1;
        }
        case 5:
        {
            // end
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

static int transit_bool_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == 't') {
                return 1;
            }
            if (c == 'f') {
                return 5;
            }
            return -1;
        }
        case 1:
        {
            // "t"
            if (c == 'r') {
                return 2;
            }
            return -1;
        }
        case 2:
        {
            // "tr"
            if (c == 'u') {
                return 3;
            }
            return -1;
        }
        case 3:
        {
            // "tru"
            if (c == 'e') {
                return 4;
            }
            return -1;
        }
        case 4:
        {
            // "true"
            return -1;
        }
        case 5:
        {
            // "f"
            if (c == 'a') {
                return 6;
            }
            return -1;
        }
        case 6:
        {
            // "fa"
            if (c == 'l') {
                return 7;
            }
            return -1;
        }
        case 7:
        {
            // "fal"
            if (c == 's') {
                return 8;
            }
            return -1;
        }
        case 8:
        {
            // "fals"
            if (c == 'e') {
                return 9;
            }
            return -1;
        }
        case 9:
        {
            // "false"
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

static int transit_num_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == '-') {
                return 1;
            }
            if (c == '.') {
                return 2;
            }
            if (c >= '0' && c <= '9') {
                return 3;
            }
            return -1;
        }
        case 1:
        {
            // "-"
            if (c == '.') {
                return 2;
            }
            if (c >= '0' && c <= '9') {
                return 3;
            }
            return -1;
        }
        case 2:
        {
            // "-?\."
            if (c >= '0' && c <= '9') {
                return 4;
            }
            return -1;
        }
        case 3:
        {
            // "-?[0-9]+"
            // valid
            if (c == '.') {
                return 4;
            }
            if (c >= '0' && c <= '9') {
                return 3;
            }
            return 5;
        }
        case 4:
        {
            // "-?(?:[0-9]*\.[0-9]+|[0-9]+\.[0-9]*)"
            // valid
            if (c >= '0' && c <= '9') {
                return 4;
            }
            return 5;
        }
        case 5:
        {
            // valid but meet unknown character
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

static int transit_str_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == '"') {
                return 1;
            }
            return -1;
        }
        case 1:
        {
            // first quotation mark
            if (c == '"') {
                return 4;
            }
            if (c == '\\') {
                return 2;
            }
            return 3;
        }
        case 2:
        {
            // the last is backslash
            if (c == '\\' ||
                c == '/' ||
                c == 'n' ||
                c == 'r' ||
                c == 't' ||
                c == 'b' ||
                c == 'f' ||
                c == '"') {
                return 3;
            }
            return -1;
        }
        case 3:
        {
            // the last is not backslash
            if (c == '"') {
                return 4;
            }
            if (c == '\\') {
                return 2;
            }
            return 3;
        }
        case 4:
        {
            // finish
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

static int transit_null_state(char c, int state) {
    switch (state) {
        case 0:
        {
            // ""
            if (is_ignored_char(c)) {
                return 0;
            }
            if (c == 'n') {
                return 1;
            }
            return -1;
        }
        case 1:
        {
            // "n"
            if (c == 'u') {
                return 2;
            }
            return -1;
        }
        case 2:
        {
            // "nu"
            if (c == 'l') {
                return 3;
            }
            return -1;
        }
        case 3:
        {
            // "nul"
            if (c == 'l') {
                return 4;
            }
            return -1;
        }
        case 4:
        {
            // "null"
            return -1;
        }
        default:
        {
            // invalid
            return -1;
        }
    }
}

// `s` is valid
static char *decode_string(char *s, int len) {
    int ret_len = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '\\') {
            i++;
        }
        ret_len++;
    }

    char *ret = (char *)malloc(sizeof(char) * (ret_len + 1));
    ret_len = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '\\') {
            switch (s[++i]) {
                case '\\':
                {
                    ret[ret_len++] = '\\';
                    break;
                }
                case '/':
                {
                    ret[ret_len++] = '/';
                    break;
                }
                case 'n':
                {
                    ret[ret_len++] = '\n';
                    break;
                }
                case 'r':
                {
                    ret[ret_len++] = '\r';
                    break;
                }
                case 't':
                {
                    ret[ret_len++] = '\t';
                    break;
                }
                case 'b':
                {
                    ret[ret_len++] = '\b';
                    break;
                }
                case 'f':
                {
                    ret[ret_len++] = '\f';
                    break;
                }
                case '"':
                {
                    ret[ret_len++] = '"';
                    break;
                }
            }

            continue;
        }

        ret[ret_len++] = s[i];
    }
    ret[ret_len++] = 0;
    return ret;
}

// `json_str` is valid
static json_data *parse_arr(char *json_str, int *i) {
    list *lst = lst_init(sizeof(json_data *));
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_arr_state(json_str[*i], state);

        if (state == 1) {
            (*i)++;
            json_data *sub_json = parse_general(json_str, i);

            if (sub_json) {
                lst_append(lst, &sub_json);
                state = 3;
            }
            else {
                state = 4;
            }
        }

        else if (state == 2) {
            (*i)++;
            json_data *sub_json = parse_general(json_str, i);

            lst_append(lst, &sub_json);
            state = 3;
        }

        else if (state == 5) {
            break;
        }
    }

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_ARRAY;
    ret->data = lst;
    return ret;
}

// `json_str` is valid
static json_data *parse_bool(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_bool_state(json_str[*i], state);

        if (state == 4 || state == 9) {
            break;
        }
    }

    bool value = state == 4 ? true : false;

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_BOOLEAN;
    ret->data = malloc(sizeof(bool));
    *(bool *)ret->data = value;
    return ret;
}

// `json_str` is valid
static json_data *parse_num(char *json_str, int *i) {
    double num = 0.;
    double scale = 1.;
    double sign = 1.;
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        char c = json_str[*i];

        state = transit_num_state(c, state);

        if (state == 1) {
            sign = -1.;
        }

        else if (state == 3) {
            num *= 10.;
            num += (double)(c - '0');
        }

        else if (state == 4) {
            if (c < '0' || c > '9') {
                continue;
            }

            scale *= 10.;
            num += (double)(c - '0') / scale;
        }

        else if (state == 5) {
            (*i)--;
            break;
        }
    }

    if (!json_str[*i]) {
        // state 3 or 4
        (*i)--;
    }

    num *= sign;

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_NUMBER;
    ret->data = malloc(sizeof(double));
    *(double *)ret->data = num;
    return ret;
}

// `json_str` is valid
static json_data *parse_str(char *json_str, int *i) {
    int start = -1;
    int end = -1;
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_str_state(json_str[*i], state);

        if (state == 1) {
            start = *i + 1;
        }

        else if (state == 4) {
            end = *i;
            break;
        }
    }

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_STRING;
    ret->data = decode_string(json_str + start, end - start);
    return ret;
}

static void obj_insert(table *tbl, list *keys, char *key, json_data *value) {
    if (!tbl_contain(tbl, key)) {
        tbl_set(tbl, key, &value);
        lst_append(keys, &key);
        return;
    }

    // maintain table
    json_data *old_value;
    tbl_get(tbl, key, &old_value);
    json_kill(old_value);
    tbl_set(tbl, key, &value);

    // maintain list
    int size = lst_size(keys);
    for (int i = 0; i < size; i++) {
        char *old_key;
        lst_get(keys, i, &old_key);

        if (!strcmp(old_key, key)) {
            free(old_key);
            lst_remove(keys, i, NULL);
            break;
        }
    }
    lst_append(keys, &key);
}

// `json_str` is valid
static json_data *parse_obj(char *json_str, int *i) {
    table *tbl = tbl_init(sizeof(json_data *));
    list *keys = lst_init(sizeof(char *));
    char *key;
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_obj_state(json_str[*i], state);

        if (state == 1) {
            (*i)++;
            json_data *key_json = parse_general(json_str, i);

            if (key_json) {
                key = (char *)malloc(sizeof(char) * (strlen((char *)key_json->data) + 1));
                strcpy(key, key_json->data);

                json_kill(key_json);

                state = 3;
            }
            else {
                state = 6;
            }
        }

        else if (state == 2) {
            (*i)++;
            json_data *key_json = parse_str(json_str, i);

            key = (char *)malloc(sizeof(char) * (strlen((char *)key_json->data) + 1));
            strcpy(key, key_json->data);

            json_kill(key_json);

            state = 3;
        }

        else if (state == 4) {
            (*i)++;
            json_data *value = parse_general(json_str, i);

            obj_insert(tbl, keys, key, value);

            state = 5;
        }

        else if (state == 7) {
            break;
        }
    }

    obj_data *data = (obj_data *)malloc(sizeof(obj_data));
    data->data = tbl;
    data->keys = keys;

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_OBJECT;
    ret->data = data;
    return ret;
}

// `json_str` is valid
static json_data *parse_null(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_null_state(json_str[*i], state);

        if (state == 4) {
            break;
        }
    }

    json_data *ret = (json_data *)malloc(sizeof(json_data));
    ret->type = JSON_NULL;
    ret->data = NULL;
    return ret;
}

// `json_str` is valid
static json_data *parse_general(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_general_state(json_str[*i], state);

        switch (state) {
            case 1:
            {
                return parse_obj(json_str, i);
            }

            case 2:
            {
                return parse_arr(json_str, i);
            }

            case 3:
            {
                return parse_bool(json_str, i);
            }

            case 4:
            {
                return parse_num(json_str, i);
            }

            case 5:
            {
                return parse_str(json_str, i);
            }

            case 6:
            {
                return parse_null(json_str, i);
            }
            case 7:
            {
                (*i)--;
                return NULL;
            }
            default:
            {
                break;
            }
        }
    }

    (*i)--;
    return NULL;
}

static bool arr_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_arr_state(json_str[*i], state);

        if (state == 1) {
            (*i)++;
            bool is_empty;
            if (!general_is_valid(json_str, i, &is_empty)) {
                return false;
            }

            state = is_empty ? 4 : 3;
        }

        else if (state == 2) {
            (*i)++;
            if (!general_is_valid(json_str, i, NULL)) {
                return false;
            }

            state = 3;
        }

        else if (state == 5) {
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    return false;
}

static bool bool_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_bool_state(json_str[*i], state);

        if (state == 4 || state == 9) {
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    return false;
}

static bool num_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_num_state(json_str[*i], state);

        if (state == 5) {
            (*i)--;
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    if (state == 3 || state == 4) {
        (*i)--;
        return true;
    }

    return false;
}

static bool str_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_str_state(json_str[*i], state);

        if (state == 4) {
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    return false;
}

static bool obj_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_obj_state(json_str[*i], state);

        if (state == 1) {
            int old_i = ++(*i);
            bool is_empty;
            if (!general_is_valid(json_str, i, &is_empty)) {
                return false;
            }
            if (!is_empty && !str_is_valid(json_str, &old_i)) {
                return false;
            }

            state = is_empty ? 6 : 3;
        }

        else if (state == 2) {
            (*i)++;
            if (!str_is_valid(json_str, i)) {
                return false;
            }

            state = 3;
        }

        else if (state == 4) {
            (*i)++;
            if (!general_is_valid(json_str, i, NULL)) {
                return false;
            }

            state = 5;
        }

        else if (state == 7) {
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    return false;
}

static bool null_is_valid(char *json_str, int *i) {
    int state = 0;

    for (; json_str[*i]; (*i)++) {
        state = transit_null_state(json_str[*i], state);

        if (state == 4) {
            return true;
        }

        else if (state == -1) {
            return false;
        }
    }

    return false;
}

// when `is_empty` is NULL, empty is seen invalid
static bool general_is_valid(char *json_str, int *i, bool *is_empty) {
    int state = 0;
    if (is_empty) {
        *is_empty = false;
    }

    for (; json_str[*i]; (*i)++) {
        state = transit_general_state(json_str[*i], state);

        if (state == 1) {
            return obj_is_valid(json_str, i);
        }

        if (state == 2) {
            return arr_is_valid(json_str, i);
        }

        if (state == 3) {
            return bool_is_valid(json_str, i);
        }

        if (state == 4) {
            return num_is_valid(json_str, i);
        }

        if (state == 5) {
            return str_is_valid(json_str, i);
        }

        if (state == 6) {
            return null_is_valid(json_str, i);
        }

        if (state == 7) {
            if (is_empty) {
                *is_empty = true;
                (*i)--;
                return true;
            }
            return false;
        }
    }

    if (is_empty) {
        *is_empty = true;
        (*i)--;
        return true;
    }
    return false;
}

#undef INDENT
