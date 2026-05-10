//  pqueue.c : partie implantation d'un module mutatif polymorphe pour la
//    spécification FILEP du TDA FileP(T).

#include "pqueue.h"

#if __CH_VERSION_PQUEUE_H__ != 202603L
#error Bad 'pqueue' version.
#else

//  struct pqueue, pqueue : gestion des adresses des objets dans la file de
//    de priorité par tas binaire dynamique.

#define CAPACITY_MIN 1
#define CAPACITY_MUL 2

struct pqueue {
  int (*compar)(const void *, const void *);
  const void **aref;
  size_t capacity;
  size_t length;
};

bool pqueue_is_empty(pqueue *q) {
  return q->length == 0;
}

size_t pqueue_length(pqueue *q) {
  return q->length;
}

//  pqueue_empty : tente d'allouer les ressources nécessaires pour gérer une
//    nouvelle file de priorité initialement vide. La fonction de comparaison
//    des objets via leurs références est pointée par compar. Renvoie un
//    pointeur nul en cas de dépassement de capacité. Renvoie sinon un pointeur
//    vers le contrôleur associé à la file.
pqueue *pqueue_empty(int (*compar)(const void *, const void *)) {
  pqueue *q = malloc(sizeof *q);
  if (q == nullptr) {
    return nullptr;
  }
  q->compar = compar;
  q->capacity = CAPACITY_MIN;
  q->length = 0;
  q->aref = malloc(CAPACITY_MIN * sizeof *(q->aref));
  return q;
}

//  pqueue_dispose : sans effet si *qptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de la file de priorité associée à *qptr
//    puis affecte un pointeur nul à *qptr.
void pqueue_dispose(pqueue **qptr) {
  if (*qptr == nullptr) {
    return;
  }
  free((*qptr)->aref);
  free(*qptr);
  *qptr = nullptr;
}

//  pqueue_enqueue : renvoie un pointeur nul si ref vaut un pointeur nul. Tente
//    sinon d'enfiler ref dans la file de priorité associée à q ; renvoie un
//    pointeur nul en cas de dépassement de capacité ; renvoie sinon ref.
void *pqueue_enqueue(pqueue *q, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  if (q->length == q->capacity) {
    size_t new_cap = q->capacity * CAPACITY_MUL;
    const void **temp = realloc(q->aref, new_cap * sizeof *(q->aref));
    if (temp == nullptr) {
      return nullptr;
    }
    q->aref = temp;
    q->capacity = new_cap;
  }
  q->aref[q->length] = ref;
  size_t i = q->length;
  while (i > 0) {
    size_t parent = (i - 1) / 2;
    if (q->compar(q->aref[i], q->aref[parent]) > 0) {
      const void *tmp = q->aref[i];
      q->aref[i] = q->aref[parent];
      q->aref[parent] = tmp;
    }
    i = parent;
  }
  q->length += 1;
  return (void *) ref;
}

//  pqueue_dequeue : renvoie un pointeur nul si la file de priorité associée à q
//    est vide. Défile sinon la file et renvoie la référence qui était en tête
//    de la file avant le défilement.
void *pqueue_dequeue(pqueue *q) {
  if (q->length == 0) {
    return nullptr;
  }
  const void *head = q->aref[0];
  q->length -= 1;
  q->aref[0] = q->aref[q->length];
  size_t i = 0;
  while (2 * i + 1 < q->length) {
    size_t child = 2 * i + 1;
    if (child + 1 < q->length && q->compar(q->aref[child + 1],
          q->aref[child]) > 0) {
      child += 1;
    }
    if (q->compar(q->aref[child], q->aref[i]) > 0) {
      const void *tmp = q->aref[i];
      q->aref[i] = q->aref[child];
      q->aref[child] = tmp;
    }
    i = child;
  }
  return (void *) head;
}

//  pqueue_head : renvoie un pointeur nul si la file de priorité associée à q
//    est vide. Renvoie sinon la référence qui est en tête de la file.
void *pqueue_head(pqueue *q) {
  return q->length == 0 ? nullptr : (void *) q->aref[0];
}

#endif