//  Test de toutes les fonctions du module bt.

#include <stdio.h>
#include <stdlib.h>
#include "bt.h"

#define ANSI_NORM printf("\x1b[0m")
#define ANSI_BOLD printf("\x1b[0;1m")

#define INITIALIZE(t, fun, size)                                               \
  t = fun(size);                                                               \
  if ((size) > 0 && t == nullptr) {                                            \
    fprintf(stderr, "*** Not enough memory\n");                                \
    exit(EXIT_FAILURE);                                                        \
  }                                                                            \
  ANSI_BOLD;                                                                   \
  printf("with %s = %s(%zu)\n", #t, #fun, (size_t) size);                      \
  ANSI_NORM

#define DISPLAY_SIZE_T(t, fun)                                                 \
  printf("%20s(%s) = %zu\n", #fun, #t, fun(t))

#define DISPLAY_BOOL(t, fun)                                                   \
  printf("%20s(%s) = %s\n", #fun, #t, fun(t) ? "true" : "false")

#define DISPLAY_VOID(t, fun, pre)                                              \
  printf("%20s(%s) = " pre, #fun, #t);                                         \
  fun(t);                                                                      \
  printf("\n")

#define DISPLAY_BOOL2(t1, t2, fun)                                             \
  printf("%20s(%s, %s) = %s\n", #fun, #t1, #t2, fun(t1, t2) ? "true" : "false")

#define RAND_SEED     20260128
#define SIZE          4
#define RANDOM_TIMES  4
#define SIMILAR_TIMES 5

int main() {
  srand(RAND_SEED);
  {
    bt *t = nullptr;
    INITIALIZE(t, bt_comb_left, SIZE);
    DISPLAY_VOID(t, bt_repr_graphic, "\n");
    DISPLAY_VOID(t, bt_repr_formal, );
    DISPLAY_SIZE_T(t, bt_size);
    DISPLAY_SIZE_T(t, bt_height);
    DISPLAY_SIZE_T(t, bt_distance);
    bt_dispose(&t);
  }
  {
    bt *t = nullptr;
    INITIALIZE(t, bt_comb_right, SIZE);
    DISPLAY_VOID(t, bt_repr_graphic, "\n");
    DISPLAY_VOID(t, bt_repr_formal, );
    DISPLAY_SIZE_T(t, bt_size);
    DISPLAY_SIZE_T(t, bt_height);
    DISPLAY_SIZE_T(t, bt_distance);
    bt_dispose(&t);
  }
  {
    for (int k = 0; k < RANDOM_TIMES; ++k) {
      bt *t = nullptr;
      INITIALIZE(t, bt_random, SIZE);
      DISPLAY_VOID(t, bt_repr_graphic, "\n");
      DISPLAY_VOID(t, bt_repr_formal, );
      DISPLAY_VOID(t, bt_repr_lukas, );
      DISPLAY_VOID(t, bt_repr_subtrees, );
      DISPLAY_SIZE_T(t, bt_size);
      DISPLAY_SIZE_T(t, bt_height);
      DISPLAY_SIZE_T(t, bt_distance);
      DISPLAY_BOOL(t, bt_is_skinny);
      DISPLAY_BOOL(t, bt_is_comb_left);
      DISPLAY_BOOL(t, bt_is_comb_right);
      bt_dispose(&t);
    }
  }
  {
    for (size_t k = 0; k < SIMILAR_TIMES; ++k) {
      bt *t1 = nullptr;
      bt *t2 = nullptr;
      INITIALIZE(t1, bt_random, k);
      INITIALIZE(t2, bt_random, k);
      DISPLAY_VOID(t1, bt_repr_graphic, "\n");
      DISPLAY_VOID(t2, bt_repr_graphic, "\n");
      DISPLAY_BOOL2(t1, t2, bt_is_similar);
      bt_dispose(&t1);
      bt_dispose(&t2);
    }
  }
  return EXIT_SUCCESS;
}
