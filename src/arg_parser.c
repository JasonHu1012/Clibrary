#include "arg_parser.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct arg_parser {
    table *arguments;
};

typedef struct {
    bool is_bool_option;
    // useless when `is_bool_option` is true
    arg_type type;
    bool is_parsed;
    // NULL when `is_bool_option` is true or unparsed
    void *data;
    char *help;
} option_info;

static int transit_str_to_int_state(char c, int state);
static int transit_str_to_double_state(char c, int state);

static option_info *init_option_info(bool is_bool_option, char *help, arg_type type) {
    option_info *ret = (option_info *)malloc(sizeof(option_info));
    ret->is_bool_option = is_bool_option;
    if (!is_bool_option) {
        ret->type = type;
    }
    ret->is_parsed = false;
    ret->data = NULL;
    if (help) {
        ret->help = (char *)malloc(sizeof(char) * (strlen(help) + 1));
        strcpy(ret->help, help);
    }
    else {
        ret->help = NULL;
    }
    return ret;
}

static void kill_option_info(option_info *info) {
    free(info->help);
    if (info->data) {
        free(info->data);
    }
    free(info);
}

arg_parser *arg_init() {
    arg_parser *ret = (arg_parser *)malloc(sizeof(arg_parser));
    ret->arguments = tbl_init(sizeof(option_info *));
    return ret;
}

void arg_kill(arg_parser *arg) {
    table *tbl = arg->arguments;

    char **keys = tbl_keys(tbl);
    int size = tbl_size(tbl);
    for (int i = 0; i < size; i++) {
        option_info *info;
        tbl_get(tbl, keys[i], &info);

        kill_option_info(info);
        free(keys[i]);
    }

    free(keys);
    tbl_kill(tbl);
    free(arg);
}

static bool is_help_option(char *option) {
    return strcmp(option, "-h") == 0 || strcmp(option, "--help") == 0;
}

static bool is_short_option(char *option) {
    return strlen(option) == 2 &&
        option[0] == '-' &&
        option[1] != '-';
}

static bool is_long_option(char *option) {
    int len = strlen(option);

    if (len < 3) {
        return false;
    }
    if (option[0] != '-' || option[1] != '-') {
        return false;
    }
    for (int i = 2; i < len; i++) {
        if (option[i] == '-') {
            return false;
        }
    }
    return true;
}

static bool is_single_option(char *option) {
    return is_short_option(option) || is_long_option(option);
}

static void kill_old_option(table *tbl, char *option) {
    option_info *info = NULL;
    tbl_get(tbl, option, &info);

    if (info) {
        kill_option_info(info);
    }
}

void arg_register(arg_parser *arg, char *option, char *help, arg_type type) {
    assert(!is_help_option(option));
    assert(is_single_option(option));

    option_info *info = init_option_info(false, help, type);
    table *tbl = arg->arguments;

    kill_old_option(tbl, option);

    tbl_set(tbl, option, &info);
}

void arg_register_bool(arg_parser *arg, char *option, char *help) {
    assert(!is_help_option(option));
    assert(is_single_option(option));

    option_info *info = init_option_info(true, help, ARG_INT /* useless */);
    table *tbl = arg->arguments;

    kill_old_option(tbl, option);

    tbl_set(tbl, option, &info);
}

static bool is_combined_option(char *option) {
    int len = strlen(option);

    if (len < 3) {
        return false;
    }
    if (option[0] != '-') {
        return false;
    }
    for (int i = 1; i < len; i++) {
        if (option[i] == '-') {
            return false;
        }
    }
    return true;
}

static void print_help(table *tbl, char *name, FILE *file) {
    char **keys = tbl_keys(tbl);
    int size = tbl_size(tbl);

    fprintf(file, "usage: %s [options]\n\n", name); 
    fprintf(file, "options:\n");

    for (int i = 0; i < size; i++) {
        option_info *info;
        tbl_get(tbl, keys[i], &info);

        if (info->is_bool_option) {
            fprintf(file, "  %s", keys[i]);
        }
        else {
            switch (info->type) {
                case ARG_INT:
                {
                    fprintf(file, "  %s <integer>", keys[i]);
                    break;
                }

                case ARG_DOUBLE:
                {
                    fprintf(file, "  %s <decimal>", keys[i]);
                    break;
                }

                case ARG_STRING:
                {
                    fprintf(file, "  %s <string>", keys[i]);
                    break;
                }
            }
        }

        if (info->help && strlen(info->help) > 0) {
            fprintf(file, ": %s", info->help);
        }
        fprintf(file, "\n");

        free(keys[i]);
    }

    free(keys);
}

static bool str_to_int(char *str, int *dst) {
    int ret = 0;
    int sign = 1;
    int state = 0;

    for (int i = 0; str[i]; i++) {
        state = transit_str_to_int_state(str[i], state);

        if (state == -1) {
            return false;
        }

        if (state == 1) {
            sign = -1;
        }
        else if (state == 3) {
            ret *= 10;
            ret += str[i] - '0';
        }
    }

    if (state != 2 && state != 3) {
        return false;
    }

    ret *= sign;
    memcpy(dst, &ret, sizeof(int));
    return true;
}

static bool str_to_double(char *str, double *dst) {
    double ret = 0.;
    double scale = 1.;
    double sign = 1.;
    int state = 0;

    for (int i = 0; str[i]; i++) {
        state = transit_str_to_double_state(str[i], state);

        if (state == -1) {
            return false;
        }

        if (state == 1) {
            sign = -1.;
        }
        else if (state == 3) {
            ret *= 10.;
            ret += (double)(str[i] - '0');
        }
        else if (state == 4 && str[i] != '.') {
            scale *= 10.;
            ret += (double)(str[i] - '0') / scale;
        }
    }

    if (state != 3 && state != 4) {
        return false;
    }

    ret *= sign;
    memcpy(dst, &ret, sizeof(double));
    return true;
}

static bool parse_single_option(option_info *info, int argc, char **argv, int *argv_i) {
    if (info->is_bool_option) {
        info->is_parsed = true;
        return true;
    }

    if (*argv_i == argc - 1) {
        return false;
    }

    switch (info->type) {
        case ARG_INT:
        {
            int data;
            if (!str_to_int(argv[*argv_i + 1], &data)) {
                return false;
            }

            info->is_parsed = true;
            info->data = malloc(sizeof(int));
            memcpy(info->data, &data, sizeof(int));

            break;
        }

        case ARG_DOUBLE:
        {
            double data;
            if (!str_to_double(argv[*argv_i + 1], &data)) {
                return false;
            }

            info->is_parsed = true;
            info->data = malloc(sizeof(double));
            memcpy(info->data, &data, sizeof(double));

            break;
        }

        case ARG_STRING:
        {
            info->is_parsed = true;
            info->data = malloc(sizeof(char) * (strlen(argv[*argv_i + 1]) + 1));
            strcpy(info->data, argv[*argv_i + 1]);

            break;
        }
    }

    // only modify argv_i when parsed successfully
    (*argv_i)++;

    return true;
}

void arg_parse(arg_parser *arg, int argc, char **argv) {
    table *tbl = arg->arguments;
    bool is_valid_argv = true;

    for (int i = 1; i < argc; i++) {
        if (is_help_option(argv[i])) {
            print_help(tbl, argv[0], stdout);
            exit(0);
        }

        if (is_single_option(argv[i])) {
            option_info *info = NULL;
            tbl_get(tbl, argv[i], &info);

            if (!info) {
                is_valid_argv = false;
                break;
            }

            if (!parse_single_option(info, argc, argv, &i)) {
                is_valid_argv = false;
                break;
            }
        }

        else if (is_combined_option(argv[i])) {
            char option[3];
            for (int j = 1; argv[i][j]; j++) {
                sprintf(option, "-%c", argv[i][j]);

                option_info *info = NULL;
                tbl_get(tbl, option, &info);

                if (!info) {
                    is_valid_argv = false;
                    break;
                }
                if (!info->is_bool_option) {
                    is_valid_argv = false;
                    break;
                }

                info->is_parsed = true;
            }
        }

        else {
            is_valid_argv = false;
            break;
        }
    }

    if (!is_valid_argv) {
        print_help(tbl, argv[0], stderr);
        exit(1);
    }
}

bool arg_is_parsed(arg_parser *arg, char *option) {
    option_info *info = NULL;
    tbl_get(arg->arguments, option, &info);

    return info ? info->is_parsed : false;
}

void arg_get(arg_parser *arg, char *option, void *dst) {
    option_info *info = NULL;
    tbl_get(arg->arguments, option, &info);

    if (!info) {
        return;
    }

    if (info->is_bool_option) {
        memcpy(dst, &info->is_parsed, sizeof(bool));
        return;
    }

    if (!info->is_parsed) {
        return;
    }

    switch (info->type) {
        case ARG_INT:
        {
            memcpy(dst, info->data, sizeof(int));
            break;
        }

        case ARG_DOUBLE:
        {
            memcpy(dst, info->data, sizeof(double));
            break;
        }

        case ARG_STRING:
        {
            *(char **)dst = malloc(sizeof(char) * (strlen(info->data) + 1));
            strcpy(*(char **)dst, (char *)info->data);
            break;
        }
    }
}

// draw FSM to understand
static int transit_str_to_int_state(char c, int state) {
    switch (state) {
        case -1:
        {
            // invalid
            return -1;
        }
        case 0:
        {
            // ""
            if (c == '0') {
                return 2;
            }
            if (c == '-') {
                return 1;
            }
            if (c >= '1' && c <= '9') {
                return 3;
            }
            return -1;
        }
        case 1:
        {
            // "-"
            if (c == '0') {
                return 2;
            }
            if (c >= '1' && c <= '9') {
                return 3;
            }
            return -1;
        }
        case 2:
        {
            // "-?0"
            return -1;
        }
        case 3:
        {
            // "-?[1-9][0-9]*"
            if (c >= '0' && c <= '9') {
                return 3;
            }
            return -1;
        }
        default:
        {
            return -1;
        }
    }
}

// draw FSM to understand
static int transit_str_to_double_state(char c, int state) {
    switch (state) {
        case -1:
        {
            // invalid
            return -1;
        }
        case 0:
        {
            // ""
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
            if (c == '.') {
                return 4;
            }
            if (c >= '0' && c <= '9') {
                return 3;
            }
            return -1;
        }
        case 4:
        {
            // "-?(?:[0-9]*\.[0-9]+|[0-9]+\.[0-9]*)"
            if (c >= '0' && c <= '9') {
                return 4;
            }
            return -1;
        }
        default:
        {
            return -1;
        }
    }
}
