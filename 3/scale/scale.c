//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#include <errno.h>
#include <stdint.h>
#include "scale.h"

#if __CH_VERSION_SCALE_H__ != 202602L
#error Bad 'scale' version.
#else

struct scale {
  uint16_t valmax;
  uint16_t *array;
};

static void *scale__set(scale *s, uint16_t n) {
  uint16_t *p = s->array + n;
  *p = n;
  return (char *) p + (rand() % 2);
}

uint16_t *scale__get(const void *p) {
  return (uint16_t *) ((unsigned long long int) p & ~1ULL);
}

scale *scale_open(int valmax) {
  if (valmax > UINT16_MAX - 1) {
    return nullptr;
  }
  scale *s = malloc(sizeof *s);
  if (s == nullptr) {
    return nullptr;
  }
  s->valmax = (uint16_t) valmax;
  s->array = malloc((uint16_t) (valmax + 1) * sizeof(uint16_t));
  if (s->array == nullptr) {
    free(s);
    return nullptr;
  }
  if (s->array != scale__get((char *) s->array + 1)) {
    fprintf(stderr, ">>> Internal error. <<<\n");
    abort();
  }
  return s;
}

void scale_close(scale **sptr) {
  if (*sptr == nullptr) {
    return;
  }
  free((*sptr)->array);
  free(*sptr);
  *sptr = nullptr;
}

void scale_rand(scale *s, const void **pp) {
  *pp = scale__set(s, (uint16_t) (rand() % (s->valmax + 1)));
}

int scale_get(scale *s, const void **pp) {
  long long int n;
  errno = 0;
  int r = scanf("%lld", &n);
  if (r == EOF) {
    return EOF;
  }
  if (r == 0 || errno != 0 || n < 0 || n > s->valmax) {
    return 0;
  }
  *pp = scale__set(s, (uint16_t) n);
  return 1;
}

void scale_value(const void *p, int *valptr) {
  *valptr = *scale__get(p);
}

int scale_compar(const void *p1, const void *p2) {
  uint16_t *q1 = scale__get(p1);
  uint16_t *q2 = scale__get(p2);
  return (q1 > q2) - (q1 < q2);
}

#endif
