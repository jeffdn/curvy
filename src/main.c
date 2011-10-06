/* main.c
 * Copyright (c) 2006
 * Jeff Nettleton
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>

#include "curvy.h"

int main(int argc, char *argv[])
{
    struct curvy *cv;
    int c, index;
    int x, y;
    struct option options[] = {
        {"help", no_argument, NULL, 'h'},
        {"qual-max", required_argument, NULL, 'q'},
        {"width", required_argument, NULL, 'w'},
        {"height", required_argument, NULL, 'h'},
        {"background", required_argument, NULL, 'g'},
        {"foreground", required_argument, NULL, 'f'},
        {"output", required_argument, NULL, 'o'},
        {"special", required_argument, NULL, 's'},
        {"a", required_argument, NULL, 'a'},
        {"b", required_argument, NULL, 'b'},
        {"c", required_argument, NULL, 'c'},
        {"d", required_argument, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };

    cv = curvy_new();

    /* set up some default values */
    cv->max = 8192;
    cv->output = strdup("curvy.png");
    cv->fg_color = 0x000000;
    cv->bg_color = 0xffffff;
    cv->width = 1280;
    cv->height = 1024;
    cv->a = 3.0;
    cv->b = -2.123;
    cv->c = 1.7243;
    cv->d = -4.719;

    while (1) {
        c = getopt_long(argc, argv, "q:w:h:g:f:o:a:s:b:c:d:h", options,
                        &index);

        if (c < 0) {
            break;
        }

        switch (c) {
        case 'q':
            cv->max = strtol(optarg, NULL, 0);

            if (cv->max > 8192)
                cv->max = 8192;

            if (cv->max < 255)
                cv->max = 255;

            break;

        case 'w':
            cv->width = strtol(optarg, NULL, 0);
            break;

        case 'h':
            cv->height = strtol(optarg, NULL, 0);
            break;

        case 'g':
            cv->bg_color = strtol(optarg, NULL, 16);    /* hexadecimal */
            break;

        case 'f':
            cv->fg_color = strtol(optarg, NULL, 16);    /* hexadecimal */
            break;

        case 'o':
            cv->output = strdup(optarg);
            break;

        case 'a':
            cv->a = strtod(optarg, NULL);
            break;

        case 'b':
            cv->b = strtod(optarg, NULL);
            break;

        case 'c':
            cv->c = strtod(optarg, NULL);
            break;

        case 'd':
            cv->d = strtod(optarg, NULL);
            break;

        case 's':
            cv->a = M_PI;
            cv->b = M_PI;
            cv->c = M_PI;
            cv->d = M_PI;
            break;

        default:
            break;
        }
    }

    cv->pixels = malloc(cv->width * sizeof *cv->pixels);
    if (NULL == cv->pixels) {
        fprintf(stderr, "allocating!\n");
        exit(1);
    }

    for (x = 0; x < cv->width; x++) {
        cv->pixels[x] = malloc(cv->height * sizeof *cv->pixels[x]);
        if (NULL == cv->pixels[x]) {
            fprintf(stderr, "allocating!\n");
            exit(1);
        }

        memset(cv->pixels[x], 0, cv->height * sizeof *cv->pixels[x]);
    }

    for (x = 0; x < cv->width; x++) {
        for (y = 0; y < cv->height; y++) {
            if (cv->pixels[x][y] < cv->max) {
                curvy_exec(cv,
                           ((float) x / (float) cv->width - 1.0) * 2.0,
                           ((float) y / (float) cv->height - 1.0) * 2.0);
            }
        }
    }

    curvy_write(cv);

    curvy_free(cv);

    return 0;
}

