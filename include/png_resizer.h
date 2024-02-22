#include "../libpng/include/png.h"

/**
 * Returns a 2D array of pixels after reading the PNG file with path <filename>.
 * A single pixel is represented by 4 png_byte's
 * Also stores the width and height in the arguments provided.
*/
png_bytep* read_png_file(char *filename, int *width, int *height);

/**
 * Returns a 2D array of pixels representing the resized image.
 * Note that this function stretches the image to fill all available space
*/
png_bytep* resize_stretched(
    png_bytep *orig_pixel_arr, 
    int old_width, 
    int old_height, 
    int new_width, 
    int new_height
);

/**
 * Writes <image_data> to a file <filename> in PNG format
*/
int write_png_file(char *filename, int width, int height, png_bytep *image_data);