#include "arg_parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGC 100
#define MAX_OPTION_LEN 100
#define EPSILON 1e-10

int argc;
char **argv;

bool double_equal(double a, double b) {
    double diff = a - b >= 0 ? a - b : b - a;
    return diff < EPSILON;
}

void func_test1() {
    printf("arg_init, arg_kill... ");

    arg_parser *arg = arg_init();
    arg_kill(arg);

    printf("pass\n");
}

void func_test2() {
    printf("int option... ");

    arg_parser *arg = arg_init();
    argc = 9;
    argv[1] = "--opt0";
    argv[2] = "1234";
    argv[3] = "--opt1";
    argv[4] = "-1234";
    argv[5] = "--opt2";
    argv[6] = "0";
    argv[7] = "--opt3";
    argv[8] = "-0";

    arg_register(arg, "--opt0", "", ARG_INT);
    arg_register(arg, "--opt1", "", ARG_INT);
    arg_register(arg, "--opt2", "", ARG_INT);
    arg_register(arg, "--opt3", "", ARG_INT);
    arg_parse(arg, argc, argv);

    int option[4];
    for (int i = 0; i < sizeof(option) / sizeof(int); i++) {
        option[i] = -1;
    }
    arg_get(arg, "--opt0", &option[0]);
    arg_get(arg, "--opt1", &option[1]);
    arg_get(arg, "--opt2", &option[2]);
    arg_get(arg, "--opt3", &option[3]);

    assert(option[0] == 1234);
    assert(option[1] == -1234);
    assert(option[2] == 0);
    assert(option[3] == 0);

    arg_kill(arg);

    printf("pass\n");
}

void func_test3() {
    printf("double option... ");

    arg_parser *arg = arg_init();
    argc = 21;
    argv[1] = "--opt0";
    argv[2] = "1234";
    argv[3] = "--opt1";
    argv[4] = "-1234";
    argv[5] = "--opt2";
    argv[6] = "0";
    argv[7] = "--opt3";
    argv[8] = "-0";
    argv[9] = "--opt4";
    argv[10] = "12.34";
    argv[11] = "--opt5";
    argv[12] = "-12.34";
    argv[13] = "--opt6";
    argv[14] = "12.";
    argv[15] = "--opt7";
    argv[16] = "-12.";
    argv[17] = "--opt8";
    argv[18] = ".34";
    argv[19] = "--opt9";
    argv[20] = "-.34";

    arg_register(arg, "--opt0", "", ARG_DOUBLE);
    arg_register(arg, "--opt1", "", ARG_DOUBLE);
    arg_register(arg, "--opt2", "", ARG_DOUBLE);
    arg_register(arg, "--opt3", "", ARG_DOUBLE);
    arg_register(arg, "--opt4", "", ARG_DOUBLE);
    arg_register(arg, "--opt5", "", ARG_DOUBLE);
    arg_register(arg, "--opt6", "", ARG_DOUBLE);
    arg_register(arg, "--opt7", "", ARG_DOUBLE);
    arg_register(arg, "--opt8", "", ARG_DOUBLE);
    arg_register(arg, "--opt9", "", ARG_DOUBLE);
    arg_parse(arg, argc, argv);

    double option[10];
    for (int i = 0; i < sizeof(option) / sizeof(double); i++) {
        option[i] = -1.;
    }
    arg_get(arg, "--opt0", &option[0]);
    arg_get(arg, "--opt1", &option[1]);
    arg_get(arg, "--opt2", &option[2]);
    arg_get(arg, "--opt3", &option[3]);
    arg_get(arg, "--opt4", &option[4]);
    arg_get(arg, "--opt5", &option[5]);
    arg_get(arg, "--opt6", &option[6]);
    arg_get(arg, "--opt7", &option[7]);
    arg_get(arg, "--opt8", &option[8]);
    arg_get(arg, "--opt9", &option[9]);

    assert(double_equal(option[0], 1234.));
    assert(double_equal(option[1], -1234.));
    assert(double_equal(option[2], 0.));
    assert(double_equal(option[3], -0.));
    assert(double_equal(option[4], 12.34));
    assert(double_equal(option[5], -12.34));
    assert(double_equal(option[6], 12.));
    assert(double_equal(option[7], -12.));
    assert(double_equal(option[8], .34));
    assert(double_equal(option[9], -.34));

    arg_kill(arg);

    printf("pass\n");
}

void func_test4() {
    printf("string option... ");

    arg_parser *arg = arg_init();
    argc = 5;
    argv[1] = "--opt0";
    argv[2] = "abc";
    argv[3] = "--opt1";
    argv[4] = "";

    arg_register(arg, "--opt0", "", ARG_STRING);
    arg_register(arg, "--opt1", "", ARG_STRING);
    arg_parse(arg, argc, argv);

    char *option[2];
    for (int i = 0; i < sizeof(option) / sizeof(char *); i++) {
        option[i] = NULL;
    }
    arg_get(arg, "--opt0", &option[0]);
    arg_get(arg, "--opt1", &option[1]);

    assert(strcmp(option[0], "abc") == 0);
    assert(strcmp(option[1], "") == 0);

    for (int i = 0; i < sizeof(option) / sizeof(char *); i++) {
        free(option[i]);
    }
    arg_kill(arg);

    printf("pass\n");
}

void func_test5() {
    printf("bool option... ");

    arg_parser *arg = arg_init();
    argc = 2;
    argv[1] = "--opt0";

    arg_register_bool(arg, "--opt0", "");
    arg_register_bool(arg, "--opt1", "");
    arg_parse(arg, argc, argv);

    bool option[2];
    option[0] = false;
    option[1] = true;
    arg_get(arg, "--opt0", &option[0]);
    arg_get(arg, "--opt1", &option[1]);

    assert(option[0]);
    assert(!option[1]);

    arg_kill(arg);

    printf("pass\n");
}

void func_test6() {
    printf("arg_is_parsed... ");

    arg_parser *arg = arg_init();
    argc = 4;
    argv[1] = "--opt0";
    argv[2] = "1234";
    argv[3] = "--opt1";

    arg_register(arg, "--opt0", "", ARG_INT);
    arg_register_bool(arg, "--opt1", "");
    arg_register(arg, "--opt2", "", ARG_INT);
    arg_register_bool(arg, "--opt3", "");
    arg_parse(arg, argc, argv);

    assert(arg_is_parsed(arg, "--opt0"));
    assert(arg_is_parsed(arg, "--opt1"));
    assert(!arg_is_parsed(arg, "--opt2"));
    assert(!arg_is_parsed(arg, "--opt3"));
    assert(!arg_is_parsed(arg, "--unregistered"));

    arg_kill(arg);

    printf("pass\n");
}

void func_test7() {
    printf("different option formats... ");

    arg_parser *arg = arg_init();
    argc = 10;
    argv[1] = "-a";
    argv[2] = "1";
    argv[3] = "--a";
    argv[4] = "2";
    argv[5] = "--aa";
    argv[6] = "3";
    argv[7] = "-b";
    argv[8] = "--b";
    argv[9] = "--bb";

    arg_register(arg, "-a", "", ARG_INT);
    arg_register(arg, "--a", "", ARG_INT);
    arg_register(arg, "--aa", "", ARG_INT);
    arg_register_bool(arg, "-b", "");
    arg_register_bool(arg, "--b", "");
    arg_register_bool(arg, "--bb", "");
    arg_parse(arg, argc, argv);

    assert(arg_is_parsed(arg, "-a"));
    assert(arg_is_parsed(arg, "--a"));
    assert(arg_is_parsed(arg, "--aa"));
    assert(arg_is_parsed(arg, "-b"));
    assert(arg_is_parsed(arg, "--b"));
    assert(arg_is_parsed(arg, "--bb"));

    arg_kill(arg);

    printf("pass\n");
}

void func_test8() {
    printf("unordered options... ");

    arg_parser *arg = arg_init();
    int argc = 8;
    argv[1] = "--opt2";
    argv[2] = "12.34";
    argv[3] = "--opt3";
    argv[4] = "abcd";
    argv[5] = "--opt4";
    argv[6] = "--opt1";
    argv[7] = "1234";

    arg_register(arg, "--opt1", "", ARG_INT);
    arg_register(arg, "--opt2", "", ARG_DOUBLE);
    arg_register(arg, "--opt3", "", ARG_STRING);
    arg_register_bool(arg, "--opt4", "");
    arg_parse(arg, argc, argv);

    assert(arg_is_parsed(arg, "--opt1"));
    assert(arg_is_parsed(arg, "--opt2"));
    assert(arg_is_parsed(arg, "--opt3"));
    assert(arg_is_parsed(arg, "--opt4"));

    arg_kill(arg);

    printf("pass\n");
}

void func_test9() {
    printf("combined options... ");

    arg_parser *arg = arg_init();
    argc = 4;
    argv[1] = "-a";
    argv[2] = "-bcd";
    argv[3] = "-e";

    arg_register_bool(arg, "-a", "");
    arg_register_bool(arg, "-b", "");
    arg_register_bool(arg, "-c", "");
    arg_register_bool(arg, "-d", "");
    arg_register_bool(arg, "-e", "");
    arg_parse(arg, argc, argv);

    assert(arg_is_parsed(arg, "-a"));
    assert(arg_is_parsed(arg, "-b"));
    assert(arg_is_parsed(arg, "-c"));
    assert(arg_is_parsed(arg, "-d"));
    assert(arg_is_parsed(arg, "-e"));

    arg_kill(arg);

    printf("pass\n");
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("=== start arg_parser tests ===\n"); 

    argv = (char **)malloc(sizeof(char *) * MAX_ARGC);
    argv[0] = "./a.out";

    func_test1();
    func_test2();
    func_test3();
    func_test4();
    func_test5();
    func_test6();
    func_test7();
    func_test8();
    func_test9();

    free(argv);

    printf("=== pass arg_parser tests ===\n");

    return 0;
}

#undef MAX_ARGC
#undef MAX_OPTION_LEN
#undef EPSILON
