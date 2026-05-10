//  Affiche sur la sortie standard la liste des différents mots lus sur
//    l'entrée standard dans l'ordre lexicographique selon la fonction standard
//    strcmp, chaque mot étant précédé de son nombre d'occurrences.
//  Limites :
//  - les mots sont obtenus par lecture sur l'entrée des suites consécutives
//    de longueur maximale mais majorée WORD_LENGTH_MAX de caractères qui ne
//    sont pas de la catégorie isspace ;
//  - toute suite de tels caractères de longueur strictement supérieure à
//    WORD_LENGTH_MAX se retrouve ainsi découpée en plusieurs mots.
//  Attention ! Le point suivant est à retravailler. Le laisser en l'état est
//    contraire aux exigences prônées :
//  - l'avertissement qui figure aux lignes 44-45 est une nuisance si le mot lu
//    a exactement la longueur WORD_LENGTH_MAX.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holdall.h"
#include "pqueue.h"

//  scptr_display : affiche sur la sortie standard *cptr, le caractère
//    tabulation, la chaine de caractères pointée par s et la fin de ligne.
//    Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec.
int scptr_display(const char *s, const long int *cptr);

//  rfree : libère la zone mémoire pointée par ptr et renvoie zéro.
int rfree(void *ptr);

#define STR(s)  #s
#define XSTR(s) STR(s)

#define WORD_LENGTH_MAX 31

int main() {
  int r = EXIT_SUCCESS;
  pqueue *q = pqueue_empty((int (*)(const void *, const void *)) strcmp);
  holdall *has = holdall_empty();
  if (q == nullptr
      || has == nullptr) {
    goto error_capacity;
  }
  char w[WORD_LENGTH_MAX + 1];
  while (scanf("%" XSTR(WORD_LENGTH_MAX) "s", w) == 1) {
    if (strlen(w) == WORD_LENGTH_MAX) {
      fprintf(stderr, "*** Warning: Word '%s...' possibly sliced\n", w);
    }
    char *s = malloc(strlen(w) + 1);
    if (s == nullptr) {
      goto error_capacity;
    }
    strcpy(s, w);
    if (holdall_put(has, s) != 0) {
      free(s);
      goto error_capacity;
    }
    if (pqueue_enqueue(q, s) == nullptr) {
      goto error_capacity;
    }
  }
  if (!feof(stdin)) {
    goto error_read;
  }
  fprintf(stderr, "--- Info: Number of read words (holdall): %zu\n",
      holdall_count(has));
  fprintf(stderr, "--- Info: Number of read words (pqueue): %zu\n",
      pqueue_length(q));
  {
    char *s = pqueue_dequeue(q);
    while (s != nullptr) {
      char *t;
      long int n = 0;
      do {
        ++n;
      } while ((t = pqueue_dequeue(q)) != nullptr && strcmp(s, t) == 0);
      if (scptr_display(s, &n) != 0) {
        goto error_write;
      }
      s = t;
    }
  }
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory\n");
  goto error;
error_read:
  fprintf(stderr, "*** Error: A read error occurs\n");
  goto error;
error_write:
  fprintf(stderr, "*** Error: A write error occurs\n");
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  pqueue_dispose(&q);
  if (has != nullptr) {
    holdall_apply(has, rfree);
  }
  holdall_dispose(&has);
  return r;
}

int scptr_display(const char *s, const long int *cptr) {
  return printf("%ld\t%s\n", *cptr, s) < 0;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}
