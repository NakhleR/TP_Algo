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
//    pas nul et qu'une erreur d'allocation survient, renvoie un pointeur vers
//    l'implantation d'un arbre de taille strictement inférieure à n. Renvoie
//    sinon un pointeur vers l'implantation du peigne.

#define DEFUN_CBT__COMB(fun, left_arg, right_arg)                              \
  static cbt *cbt__ ## fun(size_t n) {                                         \
    return n == 0                                                              \
      ? EMPTY()                                                                \
      : cbt__root(left_arg, right_arg);                                        \
  }

DEFUN_CBT__COMB(comb_left, cbt__comb_left(n - 1), EMPTY())
DEFUN_CBT__COMB(comb_right, EMPTY(), cbt__comb_right(n - 1))

static cbt *cbt__random(size_t n) {
  if (n == 0) {
    return EMPTY();
  }
  size_t k = (size_t) ((double) rand() / (RAND_MAX + 1.0) * (double) n);
  return cbt__root(cbt__random(k), cbt__random(n - 1 - k));
}

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

static bool cbt__is_skinny(const cbt *p) {
  return IS_EMPTY(p)
    || ((IS_EMPTY(LEFT(p)) || IS_EMPTY(RIGHT(p)))
        && cbt__is_skinny(LEFT(p))
        && cbt__is_skinny(RIGHT(p)));
}

#define DEFUN_CBT__IS_COMB(fun, child_dir, other_dir)                          \
  static bool cbt__ ## fun(const cbt *p) {                                     \
    return IS_EMPTY(p)                                                         \
      || (IS_EMPTY(NEXT(p, other_dir))                                         \
          && cbt__ ## fun(NEXT(p, child_dir)));                                \
  }

DEFUN_CBT__IS_COMB(is_comb_left, 0, 1)
DEFUN_CBT__IS_COMB(is_comb_right, 1, 0)

static bool cbt__is_similar(const cbt *p1, const cbt *p2) {
  if (IS_EMPTY(p1)) {
    return IS_EMPTY(p2);
  }
  return !IS_EMPTY(p2)
    && cbt__is_similar(LEFT(p1), LEFT(p2))
    && cbt__is_similar(RIGHT(p1), RIGHT(p2));
}

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

#define REPR_SYM_LUKAS_NODE         "a"
#define REPR_SYM_LUKAS_EMPTY        "b"
#define REPR_SYM_SUBTREES_NONEMPTY  "1"
#define REPR_SYM_SUBTREES_EMPTY     "0"

static void cbt__repr_lukas(const cbt *p) {
  if (IS_EMPTY(p)) {
    printf(REPR_SYM_LUKAS_EMPTY);
    return;
  }
  printf(REPR_SYM_LUKAS_NODE);
  cbt__repr_lukas(LEFT(p));
  cbt__repr_lukas(RIGHT(p));
}

static void cbt__repr_subtrees(const cbt *p) {
  if (IS_EMPTY(p)) {
    return;
  }
  printf(IS_EMPTY(LEFT(p))
      ? REPR_SYM_SUBTREES_EMPTY
      : REPR_SYM_SUBTREES_NONEMPTY);
  printf(IS_EMPTY(RIGHT(p))
      ? REPR_SYM_SUBTREES_EMPTY
      : REPR_SYM_SUBTREES_NONEMPTY);
  cbt__repr_subtrees(LEFT(p));
  cbt__repr_subtrees(RIGHT(p));
}

#define REPR_TAB 4

#define REPR_SYM_GRAPHIC_NODE   "O"
#define REPR_SYM_GRAPHIC_EMPTY  "|"

//  cbt__repr_graphic : affiche la représentation graphique par rotation
//    antihoraire d'un quart de tour du sous-arbre binaire p avec une
//    indentation par niveau de REPR_TAB caractères. Le niveau du sous-arbre est
//    supposé être la valeur de level.
static void cbt__repr_graphic(const cbt *p, size_t level) {
  if (IS_EMPTY(p)) {
    printf("%*s" REPR_SYM_GRAPHIC_EMPTY "\n",
        (int) (level * REPR_TAB), "");
    return;
  }
  cbt__repr_graphic(RIGHT(p), level + 1);
  printf("%*s" REPR_SYM_GRAPHIC_NODE "\n",
      (int) (level * REPR_TAB), "");
  cbt__repr_graphic(LEFT(p), level + 1);
}

//=== Type bt ==================================================================

//--- Définition bt ------------------------------------------------------------

struct bt {
  cbt *root;
};

//--- Fonctions bt -------------------------------------------------------------

#define DEFUN_BT_BUILD(fun)                                                    \
  bt *bt_ ## fun(size_t n) {                                                   \
    bt *t = malloc(sizeof *t);                                                 \
    if (t == nullptr) {                                                        \
      return nullptr;                                                          \
    }                                                                          \
    cbt *p = cbt__ ## fun(n);                                                  \
    if (n != 0 && cbt__size(p) < n) {                                          \
      cbt__dispose(p);                                                         \
      free(t);                                                                 \
      return nullptr;                                                          \
    }                                                                          \
    t->root = p;                                                               \
    return t;                                                                  \
  }

DEFUN_BT_BUILD(comb_left)
DEFUN_BT_BUILD(comb_right)
DEFUN_BT_BUILD(random)

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

#define DEFUN_BT_PRED(fun)                                                     \
  bool bt_ ## fun(bt *t) {                                                     \
    return cbt__ ## fun(t->root);                                              \
  }

DEFUN_BT_PRED(is_skinny)
DEFUN_BT_PRED(is_comb_left)
DEFUN_BT_PRED(is_comb_right)

bool bt_is_similar(bt *t1, bt *t2) {
  return cbt__is_similar(t1->root, t2->root);
}

void bt_repr_formal(bt *t) {
  cbt__repr_formal(t->root);
}

void bt_repr_lukas(bt *t) {
  cbt__repr_lukas(t->root);
}

void bt_repr_subtrees(bt *t) {
  cbt__repr_subtrees(t->root);
}

void bt_repr_graphic(bt *t) {
  cbt__repr_graphic(t->root, 0);
}

#endif
