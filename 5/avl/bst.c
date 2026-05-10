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
  int height;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define HEIGHT(p)   ((p)->height)
#define NEXT(p, d)  ((p)->next[(d) > 0])

//--- Divers -------------------------------------------------------------------

static size_t add__size_t(size_t x1, size_t x2) {
  return x1 + x2;
}

static int max__int(int x1, int x2) {
  return x1 > x2 ? x1 : x2;
}

static int min__int(int x1, int x2) {
  return x1 < x2 ? x1 : x2;
}

#ifdef AVL_VERBOSE_ROTATION
#define LOG_ROTATION(name) (void) printf(name "\n")
#else
#define LOG_ROTATION(name) ((void) 0)
#endif

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

//  cbst__size : définition.

DEFUN_CBST__MEASURE(size, add__size_t)

static int cbst__height(const cbst *p) {
  return IS_EMPTY(p) ? 0 : HEIGHT(p);
}

static void cbst__update_height(cbst *p) {
  HEIGHT(p) = 1 + max__int(cbst__height(LEFT(p)), cbst__height(RIGHT(p)));
}

static int cbst__balance(const cbst *p) {
  return IS_EMPTY(p)
    ? 0
    : cbst__height(LEFT(p)) - cbst__height(RIGHT(p));
}

static int cbst__distance(const cbst *p) {
  return IS_EMPTY(p)
    ? 0
    : 1 + min__int(cbst__distance(LEFT(p)), cbst__distance(RIGHT(p)));
}

//  cbst__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

static void cbst__rotation_left(cbst **pp) {
  cbst *p = *pp;
  cbst *q = RIGHT(p);
  RIGHT(p) = LEFT(q);
  LEFT(q) = p;
  *pp = q;
  cbst__update_height(p);
  cbst__update_height(q);
  LOG_ROTATION("left");
}

static void cbst__rotation_right(cbst **pp) {
  cbst *p = *pp;
  cbst *q = LEFT(p);
  LEFT(p) = RIGHT(q);
  RIGHT(q) = p;
  *pp = q;
  cbst__update_height(p);
  cbst__update_height(q);
  LOG_ROTATION("right");
}

static void cbst__rotation_left_right(cbst **pp) {
  cbst__rotation_left(&LEFT(*pp));
  cbst__rotation_right(pp);
}

static void cbst__rotation_right_left(cbst **pp) {
  cbst__rotation_right(&RIGHT(*pp));
  cbst__rotation_left(pp);
}

static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  int bal = cbst__balance(*pp);
  if (bal == 2) {
    if (cbst__balance(LEFT(*pp)) >= 0) {
      cbst__rotation_right(pp);
    } else {
      cbst__rotation_left_right(pp);
    }
    return 1;
  }
  if (bal == -2) {
    if (cbst__balance(RIGHT(*pp)) <= 0) {
      cbst__rotation_left(pp);
    } else {
      cbst__rotation_right_left(pp);
    }
    return 1;
  }
  return 0;
}

static void *cbst__search(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *)) {
  while (!IS_EMPTY(p)) {
    int c = compar(ref, REF(p));
    if (c == 0) {
      return (void *) REF(p);
    }
    p = NEXT(p, c);
  }
  return nullptr;
}

static void *cbst__add_endofpath(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    cbst *p = malloc(sizeof *p);
    if (p == nullptr) {
      return nullptr;
    }
    LEFT(p) = EMPTY();
    RIGHT(p) = EMPTY();
    REF(p) = ref;
    HEIGHT(p) = 1;
    *pp = p;
    return (void *) ref;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    return (void *) REF(*pp);
  }
  void *r = cbst__add_endofpath(&NEXT(*pp, c), ref, compar);
  cbst__balancing(pp);
  return r;
}

static void *cbst__remove_max(cbst **pp) {
  if (!IS_EMPTY(RIGHT(*pp))) {
    void *r = cbst__remove_max(&RIGHT(*pp));
    cbst__balancing(pp);
    return r;
  }
  cbst *p = *pp;
  const void *r = REF(p);
  *pp = LEFT(p);
  free(p);
  return (void *) r;
}

static void cbst__remove_root(cbst **pp) {
  cbst *p = *pp;
  if (IS_EMPTY(LEFT(p))) {
    *pp = RIGHT(p);
    free(p);
    return;
  }
  REF(p) = cbst__remove_max(&LEFT(p));
  cbst__balancing(pp);
}

static void *cbst__remove_climbup_left(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    return nullptr;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    const void *r = REF(*pp);
    cbst__remove_root(pp);
    return (void *) r;
  }
  void *r = cbst__remove_climbup_left(&NEXT(*pp, c), ref, compar);
  cbst__balancing(pp);
  return r;
}

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static size_t cbst__number(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t number);
//    static size_t cbst__rank(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t rank);

#define REPR__TAB 4

static void cbst__repr_graphic(const cbst *p, void (*put)(const void *),
    int level) {
  if (IS_EMPTY(p)) {
    printf("%*s|\n", level * REPR__TAB, "");
    return;
  }
  cbst__repr_graphic(RIGHT(p), put, level + 1);
  printf("%*s", level * REPR__TAB, "");
  put(REF(p));
  printf(" h=%d b=%d\n", cbst__height(p), cbst__balance(p));
  cbst__repr_graphic(LEFT(p), put, level + 1);
}

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

void *bst_add_endofpath(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__add_endofpath(&t->root, ref, t->compar);
}

void *bst_remove_climbup_left(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__remove_climbup_left(&t->root, ref, t->compar);
}

void *bst_search(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__search(t->root, ref, t->compar);
}

size_t bst_size(bst *t) {
  return cbst__size(t->root);
}

size_t bst_height(bst *t) {
  return (size_t) cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return (size_t) cbst__distance(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bst_number
//    bst_rank

void bst_repr_graphic(bst *t, void (*put)(const void *)) {
  cbst__repr_graphic(t->root, put, 0);
}

//==============================================================================

#endif
