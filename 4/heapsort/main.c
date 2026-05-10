#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STR(s)  #s
#define XSTR(s) STR(s)

#ifdef QSORT

#define SORT        qsort
#define SORT_LABEL  "GCC " XSTR(SORT)

#else

#define SORT        heapsort
#define SORT_LABEL  "a homemade " XSTR(SORT)

#include "heapsort.h"

#endif

#define LENGTH_DEFAULT        25
#define TRIALS_RATIO_DEFAULT  200000000

#if defined ECHO
#if !defined LENGTH || LENGTH <= 0 || LENGTH > LENGTH_DEFAULT
#undef LENGTH
#define LENGTH LENGTH_DEFAULT
#endif
#else
#if !defined LENGTH || LENGTH <= 0
#undef LENGTH
#define LENGTH LENGTH_DEFAULT
#endif
#if !defined TRIALS || TRIALS <= 0
#undef TRIALS
#define TRIALS (TRIALS_RATIO_DEFAULT / LENGTH)
#endif
#endif

//  int_compar : renvoie une valeur strictement négative, nulle ou strictement
//    positive selon que l'objet pointé par p1 est strictement inférieur, égal
//    ou strictement supérieur à celui pointé par p2.
int int_compar(const int *p1, const int *p2);

//  stop : affiche sur la sortie standard une invite de commande. Lit ensuite
//    des caractères sur l'entrée standard jusqu'à détecter la fin de l'entrée
//    ou obtenir 'q', 'Q' ou '\n'. Renvoie zéro si '\n' est obtenu. Provoque
//    sinon la terminaison normale du programme en signifiant à l’environnement
//    d’exécution un succès.
int stop();

int main() {
  srand(0);
  printf("--- Trials on " SORT_LABEL "\n");
#if defined ECHO
  printf("--- Type ctrl+d or enter 'q' or 'Q' to exit\n\n");
  do {
    int a[LENGTH];
    for (size_t k = 0; k < LENGTH; ++k) {
      a[k] = rand() % 100;
    }
    for (size_t k = 0; k < LENGTH; ++k) {
      printf("%3d", a[k]);
    }
    printf("\n");
    SORT(a, LENGTH, sizeof *a,
        (int (*)(const void *, const void *)) int_compar);
    for (size_t k = 0; k < LENGTH; ++k) {
      printf("%3d", a[k]);
    }
    printf("\n");
  } while (!stop());
#else
  int *a;
  if (LENGTH > PTRDIFF_MAX / sizeof *a
      || (a = malloc(LENGTH * sizeof *a)) == nullptr) {
    fprintf(stderr, "*** Too big value of " STR(LENGTH) "\n");
    exit(EXIT_FAILURE);
  }
  printf("--- " STR(LENGTH) " = %d\n", LENGTH);
  printf("--- " STR(TRIALS) " = %d\n", TRIALS);
  printf("--- Done: ---%%");
  for (int j = 0; j < TRIALS; ++j) {
    for (size_t k = 0; k < LENGTH; ++k) {
      a[k] = rand();
    }
    SORT(a, LENGTH, sizeof *a,
        (int (*)(const void *, const void *)) int_compar);
    printf("\x1b[4D%3d%%", 100 * (j + 1) / TRIALS);
    fflush(stdout);
  }
  printf("\n");
  free(a);
  return EXIT_SUCCESS;
#endif
}

int int_compar(const int *p1, const int *p2) {
  return (*p1 > *p2) - (*p1 < *p2);
}

#define PROMPT "> "

int stop() {
  printf(PROMPT);
  while (1) {
    int c = getchar();
    if (c == EOF || c == 'q' || c == 'Q') {
      printf("\n");
      exit(EXIT_SUCCESS);
    }
    if (c == '\n') {
      return 0;
    }
  }
}
