/* curvy.h
 * Copyright (c) 2006
 * Jeff Nettleton
 */

#ifndef CURVY_H
#define CURVY_H

struct curvy {
    int max;

    int width;
    int height;
    int **pixels;

    float a, b, c, d;

    int bg_color;
    int fg_color;

    char *output;
};

/* create a new curvy object */
struct curvy *curvy_new(void);
/* execute the curvy equation */
int curvy_exec(struct curvy *, float, float);
/* output the curvy image */
int curvy_write(struct curvy *);
/* free a curvy object */
void curvy_free(struct curvy *);

#endif

