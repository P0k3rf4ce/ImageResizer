#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include/png_resizer.h"
#include "libpng/include/png.h"

/**
 * Returns a 2D array of pixels after reading the PNG file with path <filename>.
 * A single pixel is represented by 4 png_byte's
 * Also stores the width and height in the arguments provided.
 * If an error occurs, returns NULL.
*/
png_bytep *read_png_file(char *filename, int *width, int *height) {
    // libpng linrearies print error messages on their own
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return NULL;

    png_infop info = png_create_info_struct(png);
    if (!info) return NULL;

    if (setjmp(png_jmpbuf(png))) return NULL;

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        return NULL;
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    if (row_pointers == NULL) abort();
    for (int y = 0; y < *height; y++) {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
        if (row_pointers[y] == NULL) abort();
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return row_pointers;
}

unsigned int ceil_div(unsigned int a, unsigned int b){
    return (a - 1) / b + 1;
}

/**
 * Returns a 2D array of pixels representing the resized image.
 * Note that this function stretches the image to fill all available space
 * If an error occurs, returns NULL.
*/
png_bytep* resize_stretched(
    png_bytep *orig_pixel_arr,
    int old_width, 
    int old_height, 
    int new_width, 
    int new_height
){
    png_bytep *resized = malloc(sizeof(png_bytep) * new_height);
    if (resized == NULL) {
        perror("malloc");
        return NULL;
    }

    // Steps store how many pixels of the old image fit into the new image
    // horizontally and vertically.
    double x_step = ((double) old_width) / new_width;
    double y_step = ((double) old_height) / new_height;

    // Factor to divide the pixel vector by when adding
    int div_factor = ceil_div(old_width, new_width) * ceil_div(old_height, new_height);

    for (int row = 0; row < new_height; row++) {
        resized[row] = malloc(sizeof(png_bytep) * new_width * 4);
        for (int col = 0; col < new_width; col++) {
            // pixel of resized at row, col is composed of pixels on the square in orig_pixel_arr defined by interval product
            // [floor(col * x_step), floor(col * x_step) + ceil(x_step)) \times [floor(row * y_step), floor(row * y_step) + ceil(y_step))
            double r = 0, g = 0, b = 0, a = 0;
            for (int orow = floor(row * y_step); orow < floor(row * y_step) + ceil(y_step); orow++){
                // Add scaled pixels from original
                for (int ocol = floor(col * x_step); ocol < floor(col * x_step) + ceil(x_step); ocol++) {
                    r += ((double)orig_pixel_arr[orow][4 * ocol]) / div_factor;
                    g += ((double)orig_pixel_arr[orow][4 * ocol + 1]) / div_factor;
                    b += ((double)orig_pixel_arr[orow][4 * ocol + 2]) / div_factor;
                    a += ((double)orig_pixel_arr[orow][4 * ocol + 3]) / div_factor;
                }
            }
            resized[row][4 * col] = (png_byte)r;
            resized[row][4 * col + 1] = (png_byte)g;
            resized[row][4 * col + 2] = (png_byte)b;
            resized[row][4 * col + 3] = (png_byte)a;
        }
    }

    return resized;
}

/**
 * Writes <image_data> to a file <filename> in PNG format
 * Returns 0 on success, otherwise prints an error message and returns 1.
*/
int write_png_file(char *filename, int width, int height, png_bytep *image_data) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror(filename);
        return 1;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return 1;

    png_infop info = png_create_info_struct(png);
    if (!info) return 1;

    if (setjmp(png_jmpbuf(png))) return 1;

    png_init_io(png, fp);

    // Set the image information
    png_set_IHDR(
        png,
        info,
        width, height,
        8,               // Bit depth
        PNG_COLOR_TYPE_RGBA, // Color type
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // Write the image data
    for (int y = 0; y < height; y++) {
        png_bytep row = image_data[y];
        png_write_row(png, row);
    }

    png_write_end(png, NULL);
    fclose(fp);

    png_destroy_write_struct(&png, &info);

    return 0;
}