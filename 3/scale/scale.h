//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#define __CH_VERSION_SCALE_H__ 202602L

#ifndef SCALE__H
#define SCALE__H

#include <stdio.h>
#include <stdlib.h>

typedef struct scale scale;

scale *scale_open(int valmax);
void scale_close(scale **sptr);

void scale_rand(scale *s, const void **pp);
int scale_get(scale *s, const void **pp);
void scale_value(const void *p, int *valptr);

int scale_compar(const void *p1, const void *p2);

#endif
