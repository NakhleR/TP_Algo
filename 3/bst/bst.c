//  bst.c : partie implantation d'un module polymorphe pour la spécification
//   ABINR du TDA ABinR(T).

#include "bst.h"

#if __CH_VERSION_BST_H__ != 202602L
#error Bad 'bst' version.
#else

//=== Type cbst ================================================================

//--- Définition cbst ----------------------------------------------------------

typedef struct cbst cbst;

struct cbst {
  cbst *next[2];
  const void *ref;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define NEXT(p, d)  ((p)->next[(d) > 0])

//--- Divers -------------------------------------------------------------------

static size_t add__size_t(size_t x1, size_t x2) {
  return x1 + x2;
}

static size_t max__size_t(size_t x1, size_t x2) {
  return x1 > x2 ? x1 : x2;
}

static size_t min__size_t(size_t x1, size_t x2) {
  return x1 < x2 ? x1 : x2;
}

//--- Fonctions cbst -----------------------------------------------------------

//  DEFUN_CBST__MEASURE : définit la fonction récursive de nom « cbst__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBST__MEASURE(fun, oper)                                         \
  static size_t cbst__ ## fun(const cbst * p) {                                \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbst__ ## fun(LEFT(p)), cbst__ ## fun(RIGHT(p)));             \
  }

//  cbst__size, cbst__height, cbst__distance : définitions.

DEFUN_CBST__MEASURE(size, add__size_t)
DEFUN_CBST__MEASURE(height, max__size_t)
DEFUN_CBST__MEASURE(distance, min__size_t)

//  cbst__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static void *cbst__add_endofpath(cbst **pp, const void *ref,
//        int (*compar)(const void *, const void *));
//    static void *cbst__remove_max(cbst **pp);
//    static void cbst__remove_root(cbst **pp);
//    static void *cbst__remove_climbup_left(cbst **pp, const void *ref,
//        int (*compar)(const void *, const void *));
//    static void *cbst__search(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *));

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static size_t cbst__number(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t number);
//    static size_t cbst__rank(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t rank);

#define REPR__TAB 4

//  ICI, PROCHAINEMENT, LA SPÉCIFICATION ET LA DÉFINITION DE :
//    static void cbst__repr_graphic(const cbst *p,
//      void (*put)(const void *ref), size_t level);

//=== Type bst =================================================================

//--- Définition bst -----------------------------------------------------------

struct bst {
  int (*compar)(const void *, const void *);
  cbst *root;
};

//--- Fonctions bst ------------------------------------------------------------

bst *bst_empty(int (*compar)(const void *, const void *)) {
  bst *t = malloc(sizeof *t);
  if (t == nullptr) {
    return nullptr;
  }
  t->compar = compar;
  t->root = EMPTY();
  return t;
}

void bst_dispose(bst **tptr) {
  if (*tptr == nullptr) {
    return;
  }
  cbst__dispose((*tptr)->root);
  free(*tptr);
  *tptr = nullptr;
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bst_add_endofpath
//    bst_remove_climbup_left
//    bst_search

size_t bst_size(bst *t) {
  return cbst__size(t->root);
}

size_t bst_height(bst *t) {
  return cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return cbst__distance(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bst_number
//    bst_rank

//  ICI, PROCHAINEMENT, LA DÉFINITION DE :
//    bst_repr_graphic

//==============================================================================

#endif
