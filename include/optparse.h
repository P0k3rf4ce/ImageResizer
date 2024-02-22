#ifndef OPTPARSE_H__
#define OPTPARSE_H__

#include <stdlib.h>

extern int width, height;
extern double scale;
extern char *dest_path;

extern int help_flag; // 1 if set, 0 otherwise

/**
 * Parse command line options.
 * Returns 0 on success, 1 on failure, or if --help is in argv. If --help is present, help text will be printed to stdout
 * In the case of a failure, an appropriate error message will be printed to stderr. In the case of success,
 * the external variables are guaranteed to be set to positive values iff provided,
 * and dest_path is guaranteed to be a string in argv.
 * Refer to the man page for available options
 */
int parse_options(int argc, char **argv, char **src_path);

#endif