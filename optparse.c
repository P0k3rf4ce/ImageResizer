#include "include/optparse.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

void usage(FILE *stream, char *prog_name){
    fprintf(stream, "Usage: %s [options] src_path\n"
                    "Refer to man page for a list of options.\n", prog_name);
}

int width = 0, height = 0;
double scale = 0.0;
char *dest_path = NULL;

int help_flag = 0;

/**
 * Parse command line options.
 * Returns 0 on success, 1 on failure, or also 1 if --help is in argv. If --help is present, help text will be printed to stdout
 * In the case of a failure, an appropriate error message will be printed to stderr. In the case of success,
 * the external variables in the header are guaranteed to be set to positive values iff provided,
 * and dest_path is guaranteed to be a string in argv.
 * Refer to the man page for available options
 */
int parse_options(int argc, char **argv, char **src_path){
    /*
    width = 0, height = 0;
    scale = 0.0;
    dest_path = NULL;
    help_flag = 0;
    */
    
    optind = 0;
    struct option longopts[] = {
        {"help", no_argument, &help_flag, 1},
        {"height", required_argument, NULL, 'h'},
        {"width", required_argument, NULL, 'w'},
        {"scale", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    int longopt_index;

    int opt;
    char **end = malloc(sizeof(char*));
    while ((opt = getopt_long(argc, argv, "h:o:s:w:", longopts, &longopt_index)) != -1){
        switch (opt){
        case 'h':
            height = strtol(optarg, end, 10);
            if (height <= 0 || **end != '\0') {
                fprintf(stderr, "Error: The height parameter must be a positive integer.\n");
                free(end);
                return 1;
            }
            break;
        case 'o':
            dest_path = optarg;
            break;
        case 's':
            scale = strtod(optarg, end);
            if (scale <= 0 || **end != '\0') {
                fprintf(stderr, "Error: The scale parameter must be a positive number.\n");
                free(end);
                return 1;
            }
            break;
        case 'w':
            width = strtol(optarg, end, 10);
            if (width <= 0 || **end != '\0') {
                fprintf(stderr, "Error: The width parameter must be a positive integer.\n");
                free(end);
                return 1;
            }
            break;
        case 0:
            // 0 iff --help
            if (longopt_index == 0) {
                usage(stdout, argv[0]);
                free(end);
                return 1;
            }
            break;
        case ':':
            fprintf(stderr, "Missing parameter for \"%s\"\n", argv[optind-1]);
            return 1;
        default:
            // Error message should be printed by default
            fprintf(stderr, "Unknown option \"%s\"\n", argv[optind-1]);
            usage(stderr, argv[0]);
            free(end);
            return 1;
            break;
        }
    }

    free(end);

    if (optind + 1 > argc) {
        fprintf(stderr, "Error: Missing src_path.\n");
        usage(stderr, argv[0]);
        return 1;
    }
    if (optind + 1 < argc) {
        fprintf(stderr, "Error: Too many arguments.\n");
        usage(stderr, argv[0]);
        return 1;
    }

    *src_path = argv[optind];
    return 0;
}