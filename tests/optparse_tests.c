#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/optparse.h"

void reset_flags() {
    width = 0, height = 0;
    scale = 0.0;
    dest_path = NULL;
    help_flag = 0;
}

// Return 1 if a test failed, return 0 if all passed.
// Use $? to access the return value in make

// All tests return 0 is passed and 1 if failed
// They also output their result to stdout

// Tests for improper inputs
int test1() {
    // Test help text
    int argc = 2;
    char *argv[] = {"resize", "--help"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 1: FAILED.\n");
        fprintf(stderr, "TEST 1: FAILED.\n"
                        "parse_option return value was not 1.\n");
        free(src_path);
        return 1;
    }

    if (!help_flag) {
        fprintf(stdout, "TEST 1: FAILED.\n");
        fprintf(stderr, "TEST 1: FAILED.\n"
                        "help_flag not set.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 1: SUCCESS.\n");
    fprintf(stderr, "TEST 1: SUCCESS.\n");

    free(src_path);
    return 0;
}

int test2() {
    // Test too many arguments
    int argc = 6;
    char *argv[] = {"resize", "--width=20", "-h", "50", "src_path", "extra"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 2: FAILED.\n");
        fprintf(stderr, "TEST 2: FAILED.\n"
                        "parse_option return value was not 1 on too many arguments.\n");
        free(src_path);
        return 1;
    }

    argc = 5;
    char *argv2[] = {"resize", "-w20", "-h", "50", "src_path"};

    reset_flags();
    res = parse_options(argc, argv2, src_path);

    if (res != 0) {
        fprintf(stdout, "TEST 2: FAILED.\n");
        fprintf(stderr, "TEST 2: FAILED.\n"
                        "parse_option thinks there's too many arguments.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 2: SUCCESS.\n");
    fprintf(stderr, "TEST 2: SUCCESS.\n");
    
    free(src_path);
    return 0;
}

int test3() {
    // Test no src_path
    int argc = 4;
    char *argv[] = {"resize", "-w20", "-h", "50"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 3: FAILED.\n");
        fprintf(stderr, "TEST 3: FAILED.\n"
                        "parse_option return value was not 1 on no src_path.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 3: SUCCESS.\n");
    fprintf(stderr, "TEST 3: SUCCESS.\n");

    free(src_path);
    return 0;
}

int test4() {
    // Test no argument provided
    int argc = 4;
    char *argv[] = {"resize", "-w20", "-h", "src_path"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 4: FAILED.\n");
        fprintf(stderr, "TEST 4: FAILED.\n"
                        "parse_option return value was not 1 on no argument provided for -h.\n");
        free(src_path);
        return 1;
    }

    argc = 4;
    char *argv2[] = {"resize", "-w", "-h50", "src_path"};

    reset_flags();
    res = parse_options(argc, argv2, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 4: FAILED.\n");
        fprintf(stderr, "TEST 4: FAILED.\n"
                        "parse_option return value was not 1 on no argument provided for -w.\n");
        free(src_path);
        return 1;
    }

    argc = 4;
    char *argv3[] = {"resize", "--scale", "-odest_path", "src_path"};

    reset_flags();
    res = parse_options(argc, argv3, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 4: FAILED.\n");
        fprintf(stderr, "TEST 4: FAILED.\n"
                        "parse_option return value was not 1 on no argument provided for --width.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 4: SUCCESS.\n");
    fprintf(stderr, "TEST 4: SUCCESS.\n");

    free(src_path);
    return 0;
}

int test5() {
    // Test for invalid argument values
    int argc = 5;
    char *argv[] = {"resize", "--width=2", "-h", "50h", "src_path"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 5: FAILED.\n");
        fprintf(stderr, "TEST 5: FAILED.\n"
                        "parse_option return value was not 1 on invalid argument value for -h.\n");
        free(src_path);
        return 1;
    }

    argc = 5;
    char *argv2[] = {"resize", "--width=w", "-h", "50", "src_path"};

    reset_flags();
    res = parse_options(argc, argv2, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 5: FAILED.\n");
        fprintf(stderr, "TEST 5: FAILED.\n"
                        "parse_option return value was not 1 on invalid argument value for --width.\n");
        free(src_path);
        return 1;
    }

    argc = 5;
    char *argv3[] = {"resize", "--width=w", "-h", "50", "--scale=-5.5", "src_path"};

    reset_flags();
    res = parse_options(argc, argv3, src_path);

    if (res != 1) {
        fprintf(stdout, "TEST 5: FAILED.\n");
        fprintf(stderr, "TEST 5: FAILED.\n"
                        "parse_option return value was not 1 on negative value for --scale.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 5: SUCCESS.\n");
    fprintf(stderr, "TEST 5: SUCCESS.\n");

    free(src_path);
    return 0;
}

// Tests for valid inputs
int test6() {
    // Test all options provided
    int argc = 7;
    char *argv[] = {"resize", "--width=20", "--height=50", "-s1.5", "-o", "dest_path", "src_path"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 0) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "parse_option return value was not 0 on valid input.\n");
        free(src_path);
        return 1;
    }

    if (help_flag) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "help_flag was set.\n");
        free(src_path);
        return 1;
    }

    if (!height) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "height was not set.\n");
        free(src_path);
        return 1;
    }
    else if (height != 50){
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "height was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    if (!width) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "width was not set.\n");
        free(src_path);
        return 1;
    }
    else if (width != 20){
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "width was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    if (!scale) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "scale was not set.\n");
        free(src_path);
        return 1;
    }
    else if (fabs(scale - 1.5) >= __DBL_EPSILON__){
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "scale was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    if (dest_path == NULL) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "dest_path was not set.\n");
        free(src_path);
        return 1;
    }
    else if (dest_path != argv[5]) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "dest_path was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    if (src_path == NULL) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "src_path was not set.\n");
        free(src_path);
        return 1;
    }
    else if (*src_path != argv[6]) {
        fprintf(stdout, "TEST 6: FAILED.\n");
        fprintf(stderr, "TEST 6: FAILED.\n"
                        "src_path was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 6: SUCCESS.\n");
    fprintf(stderr, "TEST 6: SUCCESS.\n");

    free(src_path);
    return 0;
}

int test7() {
    // Test no options provided
    int argc = 2;
    char *argv[] = {"resize", "src_path"};
    char **src_path = malloc(sizeof(char*));

    reset_flags();
    int res = parse_options(argc, argv, src_path);

    if (res != 0) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "parse_option return value was not 0 on valid input.\n");
        free(src_path);
        return 1;
    }

    if (help_flag) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "help_flag was set.\n");
        free(src_path);
        return 1;
    }

    if (height) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "height was set.\n");
        free(src_path);
        return 1;
    }

    if (width) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "width was set.\n");
        free(src_path);
        return 1;
    }

    if (scale) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "scale was set.\n");
        free(src_path);
        return 1;
    }

    if (dest_path != NULL) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "dest_path was set.\n");
        free(src_path);
        return 1;
    }

    if (src_path == NULL) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "src_path was not set.\n");
        free(src_path);
        return 1;
    }
    else if (*src_path != argv[1]) {
        fprintf(stdout, "TEST 7: FAILED.\n");
        fprintf(stderr, "TEST 7: FAILED.\n"
                        "src_path was set, but not correctly.\n");
        free(src_path);
        return 1;
    }

    fprintf(stdout, "TEST 7: SUCCESS.\n");
    fprintf(stderr, "TEST 7: SUCCESS.\n");

    free(src_path);
    return 0;
}

int main(){
    int return_flag = 0;

    if (test1()) return_flag = 1;
    if (test2()) return_flag = 1;
    if (test3()) return_flag = 1;
    if (test4()) return_flag = 1;
    if (test5()) return_flag = 1;
    if (test6()) return_flag = 1;
    if (test7()) return_flag = 1;

    return return_flag;
}