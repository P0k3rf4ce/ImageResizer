/*
 * Copyright (c) 2024 Tymofiy Sompura
 * Portions of this software are based on libpng library (https://libpng.sourceforge.io/).
 * See the following for the license information for libpng:
 *
 * libpng License:
 * This code is released under the zlib/libpng license.
 * For details, see the full license text in the file LICENSE located in the libpng source
 * distribution or at https://opensource.org/licenses/Zlib.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Image Resizer
// This program resizes PNG images with the use of the libpng library (version 1.6.40)
// Refer to the man page for more details on usage.
// Author: Tymofiy Sompura

#include <stdio.h>
#include <stdlib.h>
#include "libpng/include/png.h"
#include "include/png_resizer.h"
#include "include/optparse.h"

int main(int argc, char **argv) {

    char **filename = malloc(sizeof(char*));

    // Parse command line options
    if (parse_options(argc, argv, filename)) {
        if (help_flag) return 0;
        else return 1;
    }

    // Read PNG file
    int orig_width, orig_height;

    png_bytep *orig_pixel_arr = read_png_file(*filename, &orig_width, &orig_height);

    free(filename);

    if (orig_pixel_arr == NULL) return 1;

    // Set width, height of the resulting image based on parsed options
    int new_width, new_height;

    if (!width) {
        if (scale) new_width = (int)(orig_width * scale);
        else new_width = orig_width;
    } 
    else new_width = width;

    if (!height) {
        if (scale) new_height = (int)(orig_height * scale);
        else new_height = orig_height;
    } 
    else new_height = height;

    png_bytep *resized = resize_stretched(orig_pixel_arr, orig_width, orig_height, new_width, new_height);

    if (resized == NULL) {
        // Free orig_pixel_arr
        for (int i = 0; i < orig_height; i++){
            free(orig_pixel_arr[i]);
        }
        free(orig_pixel_arr);
        return 1;
    }

    if (dest_path == NULL) {
        dest_path = "a.png";
    }

    int write_return_value = write_png_file(dest_path, new_width, new_height, resized);

    // Free orig_pixel_arr
    for (int i = 0; i < orig_height; i++){
        free(orig_pixel_arr[i]);
    }
    free(orig_pixel_arr);

    // Free resized
    for (int i = 0; i < new_height; i++){
        free(resized[i]);
    }
    free(resized);

    if (write_return_value) return 1;

    return 0;
}
