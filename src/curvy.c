/* curvy.c
 * Copyright (c) 2006
 * Jeff Nettleton
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>

#include <gd.h>

#include "curvy.h"

/**
 * create a new curvy object
 */
struct curvy *curvy_new(void)
{
    struct curvy *cv;

    cv = malloc(sizeof *cv);
    if (NULL == cv) {
        fprintf(stderr, "allocation!\n");
        exit(1);
    }

    return cv;
}

/**
 * run the equation on a set of pixels
 * @param cv the curvy object to opearte on
 * @param x the x value to use
 * @param y the y value to use
 */
int curvy_exec(struct curvy *cv, float ver, float hor)
{
    float xn, yn;
    int ax, ay;

    xn = hor;
    yn = ver;

    if (abs(xn) > 2 || abs(yn) > 2) {
        xn = ((float) xn / (float) cv->width - 1.0) * 2.0;
        yn = ((float) yn / (float) cv->height - 1.0) * 2.0;
    }

    xn = sinf(cv->a * yn) - cosf(cv->b * xn);
    yn = sinf(cv->c * xn) - cosf(cv->d * yn);

    ax = (xn + 2.0) * ((float) cv->width / 4.0);
    ay = (yn + 2.0) * ((float) cv->height / 4.0);

    if (ax >= cv->width || ay >= cv->height) {
        /* oh shit! */
        return 1;
    }

    while (cv->pixels[ax][ay]++ < cv->max) {
        xn = sinf(cv->a * yn) - cosf(cv->b * xn);
        yn = sinf(cv->c * xn) - cosf(cv->d * yn);

        ax = (xn + 2.0) * ((float) cv->width / 4.0);
        ay = (yn + 2.0) * ((float) cv->height / 4.0);
    }

    return 1;
}

/**
 * write the curvy image
 * @param cv the curvy object to use for writing
 */
int curvy_write(struct curvy *cv)
{
    gdImage *im;
    int x, y;
    int bg[3], fg[3];
    int color = 0;
    float ratio;
    FILE *fp;

    im = gdImageCreateTrueColor(cv->width, cv->height);

    bg[0] = (cv->bg_color & 0xff0000) >> 16;
    bg[1] = (cv->bg_color & 0xff00) >> 8;
    bg[2] = (cv->bg_color & 0xff);

    fg[0] = ((cv->fg_color & 0xff0000) >> 16) - bg[0];
    fg[1] = ((cv->fg_color & 0xff00) >> 8) - bg[1];
    fg[2] = ((cv->fg_color & 0xff)) - bg[2];

    for (x = 0; x < cv->width; x++) {
        for (y = 0; y < cv->height; y++) {
            color = 0;
            ratio = (float) cv->pixels[x][y] / (float) cv->max;

            color += (int) (fg[0] * ratio + bg[0]) << 16;
            color += (int) (fg[1] * ratio + bg[1]) << 8;
            color += (int) (fg[2] * ratio + bg[2]);

            gdImageSetPixel(im, x, y, color);
        }
    }

    if (NULL == (fp = fopen(cv->output, "w"))) {
        fprintf(stderr, "opening output file!\n");
        return 0;
    }

    gdImageJpeg(im, fp, 90);

    fclose(fp);
    gdImageDestroy(im);

    return 1;
}

/**
 * destroy a curvy object
 * @param cv the object to destroy
 */
void curvy_free(struct curvy *cv)
{
    int x;

    if (cv->width) {
        for (x = 0; x < cv->width; x++) {
            if (cv->pixels[x]) {
                /* poof! */
                free(cv->pixels[x]);
            }
        }
    }

    if (cv->output)
        free(cv->output);
}

