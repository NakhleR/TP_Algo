//  heapsort.h : partie interface d'un module polymorphe pour le tri par tas.

#ifndef HEAPSORT__H
#define HEAPSORT__H

#include <stddef.h>

//  heapsort : même spécification que la fonction qsort déclarée dans l'en-tête
//    <stdlib.h> de la bibliothèque standard.
extern void heapsort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));

#endif
