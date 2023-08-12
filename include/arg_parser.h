#ifndef __ARG_PARSER.h
#define __ARG_PARSER.h

#include <stdbool.h>

typedef struct arg_parser arg_parser;

typedef enum {
    ARG_INT,    // int
    ARG_DOUBLE, // double
    ARG_STRING  // char *
} arg_type;

// initialize a command line argument parser
arg_parser *arg_init();

// kill `arg`
void *arg_kill(arg_parser *arg);

// with "-h" or "--help", options list and `help` will be printed to stderr
// therefore, it's not allowed to register "-h" or "--help"
// all options are optional, not required, and the order doesn't matter
// short option: start with "-" and followed by exactly one character
// long option: start with "--" and followed by one or more characters

// register `option` of type `type` into `arg`
// options registered with this function should be followed by a value
// example: "-a 3 --bb 1.2 --c a_string"
void arg_register(arg_parser *arg, char *option, char *help, arg_type type);

// register bool type `option` into `arg`
// options registered with this function should not be followed by any value
// short options can be combined
// example: "-a --bb --c -def"
void arg_register_bool(arg_parser *arg, char *option, char *help);

// with options registered in `arg`, parse `argv` whose length is `argc`
void arg_parse(arg_parser *arg, int argc, char **argv);

// return whether `option` in `arg` is parsed
// `option` can be unregistered
bool arg_is_parsed(arg_parser *arg, char *option);

// copy the parsed result of `option` in `arg` to `dst`
// if `option` isn't registered or is unparsed, `dst` won't be modified
// with bool type options, the result is equal to `arg_is_parsed`
// with string type options, `dst` should be (char **) and user should `free` it after usage
void arg_get(arg_parser *arg, char *option, void *dst);

#endif
