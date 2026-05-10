//  heapsort.c : partie implantation d'un module polymorphe pour le tri par tas.

#include "heapsort.h"
#include <string.h>

//  heapsort_down : il est supposé que base est l'adresse du premier composant
//    d'un tableau de longueur nmemb et de taille de composants size, que
//    nmemb >= 1, que k <= nmemb - 1 et que le tableau est un maximier sur
//    [ k + 1 ... nmemb - 1 ] relativement à la fonction de comparaison pointée
//    par compar. Descend le composant d'indice k à la bonne place de manière à
//    faire du tableau un maximier sur [ k ... nmemb - 1 ].
static void heapsort_down(char *base, size_t nmemb, size_t size,
                          int (*compar)(const void *, const void *), size_t k)
{
    while (2 * k + 1 < nmemb)
    {
        size_t child = 2 * k + 1;
        if (child + 1 < nmemb && compar(base + (child + 1) * size, base + child * size) > 0)
        {
            child += 1;
        }
        if (compar(base + child * size, base + k * size) > 0)
        {
            char tmp[size];
            memcpy(tmp, base + k * size, size);
            memcpy(base + k * size, base + child * size, size);
            memcpy(base + child * size, tmp, size);
        }
        k = child;
    }
}

//  heapsort_down_from_0 : il est supposé que base est l'adresse du premier
//    composant d'un tableau de longueur n + 1 et de taille de composants size,
//    que n >= 1 et que le tableau est un maximier sur [ 0 ... n ] relativement
//    à la fonction de comparaison pointée par compar. Effectue l'échange des
//    composants d'indices n et 0 puis la descente du composant d'indice 0 à la
//    bonne place de manière à faire du tableau un maximier sur [ 0 ... n - 1 ].
static void heapsort_down_from_0(char *base, size_t n, size_t size,
                                 int (*compar)(const void *, const void *))
{
    char tmp[size];
    memcpy(tmp, base, size);
    memcpy(base, base + n * size, size);
    memcpy(base + n * size, tmp, size);
    heapsort_down(base, n, size, compar, 0);
}

void heapsort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *))
{
    if (nmemb <= 1)
    {
        return;
    }
    for (size_t k = nmemb / 2; k > 0; k--)
    {
        heapsort_down((char *)base, nmemb, size, compar, k - 1);
    }
    for (size_t n = nmemb - 1; n >= 1; n--)
    {
        heapsort_down_from_0((char *)base, n, size, compar);
    }
}
