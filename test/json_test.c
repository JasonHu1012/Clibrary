#include "json.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func_test1() {
    char **valid_input = (char *[]){
        "\r\n\t 123 \r\n\t\n ",
        "1.23",
        "-.123",
        "123.",
        "true\t\t\r\n  ",
        "\n\r\n false",
        "null",
        "\n  \"\\\"abc\\\\ \\/\\r\\nabc\\f\\babc\\\"\\tabc  \"",
        "\r[{\"a\":1,\"b\":\nfalse}\n,\"abc\",\r\n{\"a\":1,\"b\":2}]",
        " {\"a\\t\\\" \":\n [\t 123\n ,true \t], \"abc\":null}\n",
        "{}"
    };
    char **invalid_input = (char *[]){
        "",
        "-",
        ".",
        "--123",
        "1 23",
        "123 123",
        "True",
        "fa lse",
        "nul",
        "\"abc\\abc\"",
        "\"abc\"abc\"",
        "\"abc",
        "[123,]",
        "[123,123",
        "{123}",
        "{\"a\":123 ",
        "{\"a\":123,}",
        "{\"a\":}",
        "{\"a\"123}",
        "{\"a\":123} 123"
    };

    printf("json_is_valid... ");

    // count valid test cases above
    for (int i = 0; i < 11; i++) {
        assert(json_is_valid(valid_input[i]));
    }
    // count invalid test cases above
    for (int i = 0; i < 20; i++) {
        assert(!json_is_valid(invalid_input[i]));
    }

    printf("pass\n");
}

void func_test2() {
    char **input = (char *[]){
        "123",
        "true",
        "null",
        "\"abc\"",
        "[123,true,{\"a\":1},{},null]",
        "{\"a\":1,\"b\":[1,{\"a\":true,\"b\":[\"a\"]},[]],\"c\":{\"a\":[[{}]]}}"
    };

    printf("json_parse, json_kill... ");

    // count test cases above
    for (int i = 0; i < 6; i++) {
        json_data *json = json_parse(input[i]);
        json_kill(json);
    }

    printf("pass\n");
}

void func_test3() {
    char **input = (char *[]){
        "123 \n",
        "  true",
        "null",
        " \t\"abc\\/\\\\\\\"abc\\ntn\"",
        "\r{  \"key\\n1\":{\"key1\":[\t\ttrue],\"key2\":\t123},\n\n\"key2\":[]}    "
    };
    // check INDENT in src/json.c
    char **output = (char *[]){
        "123",
        "true",
        "null",
        "\"abc/\\\\\\\"abc\\ntn\"",
        "{\n\
  \"key\\n1\": {\n\
    \"key1\": [\n\
      true\n\
    ],\n\
    \"key2\": 123\n\
  },\n\
  \"key2\": [\n\
  ]\n\
}"
    };

    printf("json_to_str (not including floating number)... ");

    // count test cases above
    for (int i = 0; i < 5; i++) {
        json_data *json = json_parse(input[i]);
        char *json_str = json_to_str(json, false);

        assert(!strcmp(json_str, output[i]));

        free(json_str);
        json_kill(json);
    }

    printf("pass\n");
}

void func_test4() {
    char *input = "{\n\
  \"key2\": {\n\
    \"key3\": 123,\n\
    \"key2\": true,\n\
    \"key1\": [\n\
      \"string\",\n\
      null,\n\
      \"string\"\n\
    ]\n\
  },\n\
  \"key3\": \"string\",\n\
  \"key1\": [\n\
    123,\n\
    123\n\
  ]\n\
}";
    char *output = "{\n\
  \"key1\": [\n\
    123,\n\
    123\n\
  ],\n\
  \"key2\": {\n\
    \"key1\": [\n\
      \"string\",\n\
      null,\n\
      \"string\"\n\
    ],\n\
    \"key2\": true,\n\
    \"key3\": 123\n\
  },\n\
  \"key3\": \"string\"\n\
}";

    printf("json_to_str with sort... ");

    json_data *json = json_parse(input);
    char *json_str = json_to_str(json, true);

    assert(!strcmp(json_str, output));

    free(json_str);
    json_kill(json);

    printf("pass\n");
}

void func_test5() {
    printf("json_type... ");

    json_data *obj_json = json_parse("{\"key\": \"value\"}");
    json_data *arr_json = json_parse("[1, 2, 3]");
    json_data *bool_json = json_parse("true");
    json_data *num_json = json_parse("123");
    json_data *str_json = json_parse("\"string\"");
    json_data *null_json = json_parse("null");

    assert(json_type(obj_json) == JSON_OBJECT);
    assert(json_type(arr_json) == JSON_ARRAY);
    assert(json_type(bool_json) == JSON_BOOLEAN);
    assert(json_type(num_json) == JSON_NUMBER);
    assert(json_type(str_json) == JSON_STRING);
    assert(json_type(null_json) == JSON_NULL);

    json_kill(obj_json);
    json_kill(arr_json);
    json_kill(bool_json);
    json_kill(num_json);
    json_kill(str_json);
    json_kill(null_json);

    printf("pass\n");
}

int str_cmp(void const *a, void const *b) {
    return strcmp(*(char **)a, *(char **)b);
}

void func_test6() {
    printf("json_obj_size, json_obj_keys... ");

    json_data *json1 = json_parse("{\"key1\": 1, \"key2\": 2, \"key3\": 3}");
    int size1 = json_obj_size(json1);
    char **keys1 = json_obj_keys(json1);
    qsort(keys1, size1, sizeof(char *), str_cmp);
    json_data *json2 = json_parse("{}");
    int size2 = json_obj_size(json2);
    char **keys2 = json_obj_keys(json2);

    // check above
    assert(size1 == 3);
    assert(!strcmp(keys1[0], "key1"));
    assert(!strcmp(keys1[1], "key2"));
    assert(!strcmp(keys1[2], "key3"));
    assert(size2 == 0);

    for (int i = 0; i < size1; i++) {
        free(keys1[i]);
    }
    free(keys1);
    json_kill(json1);
    free(keys2);
    json_kill(json2);

    printf("pass\n");
}

void func_test7() {
    printf("json_arr_size... ");

    json_data *json1 = json_parse("[1, 2, 3]");
    int size1 = json_arr_size(json1);
    json_data *json2 = json_parse("[]");
    int size2 = json_arr_size(json2);

    // check above
    assert(size1 == 3);
    assert(size2 == 0);

    json_kill(json1);
    json_kill(json2);

    printf("pass\n");
}

void func_test8() {
    printf("json_bool_get... ");

    json_data *json1 = json_parse("true");
    json_data *json2 = json_parse("false");

    // check above
    assert(json_bool_get(json1));
    assert(!json_bool_get(json2));

    json_kill(json1);
    json_kill(json2);

    printf("pass\n");
}

void func_test9() {
    printf("json_num_get... ");

    json_data *json1 = json_parse("0");
    json_data *json2 = json_parse("-0");
    json_data *json3 = json_parse("0.");
    json_data *json4 = json_parse("-0.");
    json_data *json5 = json_parse(".0");
    json_data *json6 = json_parse("-.0");
    json_data *json7 = json_parse("125");
    json_data *json8 = json_parse("-125");
    json_data *json9 = json_parse("125.");
    json_data *json10 = json_parse("-125.");
    json_data *json11 = json_parse("1.25");
    json_data *json12 = json_parse("-1.25");
    json_data *json13 = json_parse(".125");
    json_data *json14 = json_parse("-.125");

    // check above
    assert(json_num_get(json1) == 0.);
    assert(json_num_get(json2) == 0.);
    assert(json_num_get(json3) == 0.);
    assert(json_num_get(json4) == 0.);
    assert(json_num_get(json5) == 0.);
    assert(json_num_get(json6) == 0.);
    assert(json_num_get(json7) == 125.);
    assert(json_num_get(json8) == -125.);
    assert(json_num_get(json9) == 125.);
    assert(json_num_get(json10) == -125.);
    assert(json_num_get(json11) == 1.25);
    assert(json_num_get(json12) == -1.25);
    assert(json_num_get(json13) == .125);
    assert(json_num_get(json14) == -.125);

    json_kill(json1);
    json_kill(json2);
    json_kill(json3);
    json_kill(json4);
    json_kill(json5);
    json_kill(json6);
    json_kill(json7);
    json_kill(json8);
    json_kill(json9);
    json_kill(json10);
    json_kill(json11);
    json_kill(json12);
    json_kill(json13);
    json_kill(json14);

    printf("pass\n");
}

void func_test10() {
    printf("json_str_get... ");

    json_data *json = json_parse("\"abc\\n\\tabc\\r\\\\\\/\\\"  \\b\\fabc\"");
    char *json_str = json_str_get(json);

    assert(!strcmp(json_str, "abc\n\tabc\r\\/\"  \b\fabc"));

    free(json_str);
    json_kill(json);

    printf("pass\n");
}

void func_test11() {
    printf("json_obj_get... ");

    json_data *json = json_parse("{\
  \"key1\": 123,\
  \"key2\": false,\
  \"key3\": null,\
  \"key4\": \"abc\",\
  \"key5\": [\
    1,\
    2\
  ],\
  \"key6\": {\
    \"key1\": 1,\
    \"key2\": 2\
  }\
}");
    json_data *sub_json1;
    json_data *sub_json2;
    char *s;

    sub_json1 = json_obj_get(json, "key1");
    assert(json_type(sub_json1) == JSON_NUMBER);
    assert(json_num_get(sub_json1) == 123.);

    sub_json1 = json_obj_get(json, "key2");
    assert(json_type(sub_json1) == JSON_BOOLEAN);
    assert(!json_bool_get(sub_json1));

    sub_json1 = json_obj_get(json, "key3");
    assert(json_type(sub_json1) == JSON_NULL);

    sub_json1 = json_obj_get(json, "key4");
    s = json_str_get(sub_json1);
    assert(json_type(sub_json1) == JSON_STRING);
    assert(!strcmp(s, "abc"));
    free(s);

    sub_json1 = json_obj_get(json, "key5");
    assert(json_type(sub_json1) == JSON_ARRAY);
    assert(json_arr_size(sub_json1) == 2);
    sub_json2 = json_arr_get(sub_json1, 0);
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 1.);
    sub_json2 = json_arr_get(sub_json1, 1);
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 2.);

    sub_json1 = json_obj_get(json, "key6");
    assert(json_type(sub_json1) == JSON_OBJECT);
    assert(json_obj_size(sub_json1) == 2);
    sub_json2 = json_obj_get(sub_json1, "key1");
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 1.);
    sub_json2 = json_obj_get(sub_json1, "key2");
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 2.);

    json_kill(json);

    printf("pass\n");
}

void func_test12() {
    printf("json_arr_get... ");

    json_data *json = json_parse("[\
  123,\
  false,\
  null,\
  \"abc\",\
  [\
    1,\
    2\
  ],\
  {\
    \"key1\": 1,\
    \"key2\": 2\
  }\
]");
    json_data *sub_json1;
    json_data *sub_json2;
    char *s;

    sub_json1 = json_arr_get(json, 0);
    assert(json_type(sub_json1) == JSON_NUMBER);
    assert(json_num_get(sub_json1) == 123.);

    sub_json1 = json_arr_get(json, 1);
    assert(json_type(sub_json1) == JSON_BOOLEAN);
    assert(!json_bool_get(sub_json1));

    sub_json1 = json_arr_get(json, 2);
    assert(json_type(sub_json1) == JSON_NULL);

    sub_json1 = json_arr_get(json, 3);
    s = json_str_get(sub_json1);
    assert(json_type(sub_json1) == JSON_STRING);
    assert(!strcmp(s, "abc"));
    free(s);

    sub_json1 = json_arr_get(json, 4);
    assert(json_type(sub_json1) == JSON_ARRAY);
    assert(json_arr_size(sub_json1) == 2);
    sub_json2 = json_arr_get(sub_json1, 0);
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 1.);
    sub_json2 = json_arr_get(sub_json1, 1);
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 2.);

    sub_json1 = json_arr_get(json, 5);
    assert(json_type(sub_json1) == JSON_OBJECT);
    assert(json_obj_size(sub_json1) == 2);
    sub_json2 = json_obj_get(sub_json1, "key1");
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 1.);
    sub_json2 = json_obj_get(sub_json1, "key2");
    assert(json_type(sub_json2) == JSON_NUMBER);
    assert(json_num_get(sub_json2) == 2.);

    json_kill(json);

    printf("pass\n");
}

void func_test13() {
    printf("json_xxx_init, json_kill... ");

    json_data *obj_json = json_obj_init();
    json_data *arr_json = json_arr_init();
    json_data *bool_json = json_bool_init(true);
    json_data *num_json = json_num_init(123.);
    json_data *str_json = json_str_init("string");
    json_data *null_json = json_null_init();

    json_kill(obj_json);
    json_kill(arr_json);
    json_kill(bool_json);
    json_kill(num_json);
    json_kill(str_json);
    json_kill(null_json);

    printf("pass\n");
}

void func_test14() {
    printf("json_obj_set... ");

    json_data *json = json_obj_init();
    json_data *value1 = json_num_init(123.);
    json_data *value2 = json_null_init();
    json_data *value3 = json_arr_init();

    json_obj_set(json, "key1", value1);
    json_obj_set(json, "key2", value2);
    json_obj_set(json, "key2", value3);

    assert(json_obj_size(json) == 2);
    assert(json_obj_get(json, "key1") == value1);
    assert(json_obj_get(json, "key2") == value3);

    char **keys = json_obj_keys(json);
    qsort(keys, 2, sizeof(char *), str_cmp);
    assert(!strcmp(keys[0], "key1"));
    assert(!strcmp(keys[1], "key2"));

    json_kill(json);
    free(keys[0]);
    free(keys[1]);
    free(keys);

    printf("pass\n");
}

void func_test15() {
    printf("json_obj_remove... ");

    json_data *json = json_obj_init();
    json_data *value1 = json_num_init(123.);
    json_data *value2 = json_arr_init();

    json_obj_set(json, "key1", value1);
    json_obj_set(json, "key2", value2);

    json_obj_remove(json, "key1");

    assert(json_obj_size(json) == 1);
    assert(json_obj_get(json, "key1") == NULL);
    assert(json_obj_get(json, "key2") == value2);

    char **keys = json_obj_keys(json);
    assert(!strcmp(keys[0], "key2"));

    json_kill(json);
    free(keys[0]);
    free(keys);

    printf("pass\n");
}

void func_test16() {
    printf("json_arr_append, json_arr_set... ");

    json_data *json = json_arr_init();
    json_data *value1 = json_num_init(123.);
    json_data *value2 = json_obj_init();
    json_data *value3 = json_str_init("value");

    json_arr_append(json, value1);
    json_arr_append(json, value2);
    json_arr_set(json, 1, value3);

    assert(json_arr_size(json) == 2);
    assert(json_arr_get(json, 0) == value1);
    assert(json_arr_get(json, 1) == value3);

    json_kill(json);

    printf("pass\n");
}

void func_test17() {
    printf("json_arr_remove... ");

    json_data *json = json_arr_init();
    json_data *value1 = json_num_init(123.);
    json_data *value2 = json_obj_init();
    json_data *value3 = json_str_init("value");

    json_arr_append(json, value1);
    json_arr_append(json, value2);
    json_arr_append(json, value3);

    json_arr_remove(json, 1);

    assert(json_arr_size(json) == 2);
    assert(json_arr_get(json, 0) == value1);
    assert(json_arr_get(json, 1) == value3);

    json_kill(json);

    printf("pass\n");
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("=== start json tests ===\n");

    func_test1(); 
    func_test2(); 
    func_test3(); 
    func_test4(); 
    func_test5(); 
    func_test6(); 
    func_test7(); 
    func_test8(); 
    func_test9(); 
    func_test10(); 
    func_test11(); 
    func_test12(); 
    func_test13(); 
    func_test14(); 
    func_test15(); 
    func_test16(); 
    func_test17(); 

    printf("=== pass json tests ===\n");

    return 0;
}
