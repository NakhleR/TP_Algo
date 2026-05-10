//  bt.c : partie implantation d'un module pour la spécification ABIN du TDA
//    ABin.

#include "bt.h"

#if __CH_VERSION_BT_H__ != 202501L
#error Bad 'bt' version.
#else

//=== Type cbt =================================================================

//--- Définition cbt -----------------------------------------------------------

typedef struct cbt cbt;

struct cbt {
  cbt *next[2];
};

//--- Raccourcis cbt -----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
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

//--- Fonctions cbt ------------------------------------------------------------

//  cbt__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbt__dispose(cbt *p) {
  if (!IS_EMPTY(p)) {
    cbt__dispose(LEFT(p));
    cbt__dispose(RIGHT(p));
    free(p);
  }
}

//  cbt__root : tente d'implanter l'arbre binaire dont les deux sous-arbres
//    gauche et droit sont pointés par left et right. En cas d'erreur
//    d'allocation, libère les ressources allouées à left et right et renvoie
//    l'arbre vide. Renvoie sinon le pointeur vers l'implantation de l'arbre.
static cbt *cbt__root(cbt *left, cbt *right) {
  cbt *p = malloc(sizeof *p);
  if (p == nullptr) {
    cbt__dispose(left);
    cbt__dispose(right);
    return EMPTY();
  }
  LEFT(p) = left;
  RIGHT(p) = right;
  return p;
}

//  cbt__comb_left : tente d'implanter un peigne gauche de taille n. Si n n'est
//    pas nul et qu'une erreur d'allocation survient, renvoie l'arbre vide.
//    Renvoie sinon un pointeur vers l'implantation du peigne.
static cbt *cbt__comb_left(size_t n) {
  return n == 0
    ? EMPTY()
    : cbt__root(cbt__comb_left(n - 1), EMPTY());
}

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static cbt *cbt__comb_right(size_t n);
//    static cbt *cbt__random(size_t n);

//  DEFUN_CBT__MEASURE : définit la fonction récursive de nom « cbt__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBT__MEASURE(fun, oper)                                          \
  static size_t cbt__ ## fun(const cbt * p) {                                  \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbt__ ## fun(LEFT(p)), cbt__ ## fun(RIGHT(p)));               \
  }

//  cbt__size, cbt__height, cbt__distance : définition.

DEFUN_CBT__MEASURE(size, add__size_t)
DEFUN_CBT__MEASURE(height, max__size_t)
DEFUN_CBT__MEASURE(distance, min__size_t)

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static bool cbt__is_skinny(const cbt *p);
//    static bool cbt__is_comb_left(const cbt *p);
//    static bool cbt__is_comb_right(const cbt *p);
//    static bool cbt__is_similar(const cbt *p1, const cbt *p2);

//  cbt__repr_formal : affiche la représentation formelle de l'arbre binaire
//    pointé par p.

#define REPR_SYM_FORMAL_LPAR  "("
#define REPR_SYM_FORMAL_RPAR  ")"
#define REPR_SYM_FORMAL_SEP   " "

static void cbt__repr_formal(const cbt *p) {
  printf(REPR_SYM_FORMAL_LPAR);
  if (!IS_EMPTY(p)) {
    cbt__repr_formal(LEFT(p));
    printf(REPR_SYM_FORMAL_SEP);
    cbt__repr_formal(RIGHT(p));
  }
  printf(REPR_SYM_FORMAL_RPAR);
}

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static void cbt__repr_lukas(const cbt *p);
//    static void cbt__repr_subtrees(const cbt *p);

#define REPR_TAB 4

//  cbt__repr_graphic : affiche la représentation graphique par rotation
//    antihoraire d'un quart de tour du sous-arbre binaire p avec une
//    indentation par niveau de REPR_TAB caractères. Le niveau du sous-arbre est
//    supposé être la valeur de level.

//  ICI, PROCHAINEMENT, LA DÉFINITION DE :
//    static void cbt__repr_graphic(const cbt *p, size_t level);

//=== Type bt ==================================================================

//--- Définition bt ------------------------------------------------------------

struct bt {
  cbt *root;
};

//--- Fonctions bt -------------------------------------------------------------

bt *bt_comb_left(size_t n) {
  bt *t = malloc(sizeof *t);
  if (t == nullptr) {
    return nullptr;
  }
  cbt *p = cbt__comb_left(n);
  if (n != 0 && IS_EMPTY(p)) {
    free(t);
    return nullptr;
  }
  t->root = p;
  return t;
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bt_comb_right
//    bt_random

void bt_dispose(bt **tptr) {
  if (*tptr == nullptr) {
    return;
  }
  cbt__dispose((*tptr)->root);
  free(*tptr);
  *tptr = nullptr;
}

size_t bt_size(bt *t) {
  return cbt__size(t->root);
}

size_t bt_height(bt *t) {
  return cbt__height(t->root);
}

size_t bt_distance(bt *t) {
  return cbt__distance(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bt_is_skinny
//    bt_is_comb_left
//    bt_is_comb_right
//    bt_is_similar

void bt_repr_formal(bt *t) {
  cbt__repr_formal(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bt_repr_lukas
//    bt_repr_subtrees
//    bt_repr_graphic

#endif
