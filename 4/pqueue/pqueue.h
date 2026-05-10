//  pqueue.h : partie interface d'un module mutatif polymorphe pour la
//    spécification FILEP du TDA FileP(T).

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#define __CH_VERSION_PQUEUE_H__ 202603L

#ifndef PQUEUE__H
#define PQUEUE__H

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « pqueue * » ou
//      « pqueue ** » ont un comportement indéterminé lorsque ce paramètre ou sa
//      déréférence n'est pas l'adresse d'un contrôleur préalablement renvoyée
//      avec succès par la fonction pqueue_empty et non révoquée depuis par la
//      fonction pqueue_dispose ;
//  - aucune fonction ne peut ajouter un pointeur nul à la structure de
//      données ;
//  - les fonctions de type de retour « void * » renvoient un pointeur nul en
//      cas d'échec. En cas de succès, elles renvoient une référence
//      actuellement ou auparavant stockée par la structure de données ;
//  - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire à aucun problème de la sorte.

#include <stdlib.h>

//  struct pqueue, pqueue : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer une file de priorité sur des objets
//    quelconques.
typedef struct pqueue pqueue;

//  pqueue_empty : tente d'allouer les ressources nécessaires pour gérer une
//    nouvelle file de priorité initialement vide. La fonction de comparaison
//    des objets via leurs références est pointée par compar. Renvoie un
//    pointeur nul en cas de dépassement de capacité. Renvoie sinon un pointeur
//    vers le contrôleur associé à la file.
extern pqueue *pqueue_empty(int (*compar)(const void *, const void *));

//  pqueue_dispose : sans effet si *qptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de la file de priorité associée à *qptr
//    puis affecte un pointeur nul à *qptr.
extern void pqueue_dispose(pqueue **qptr);

//  pqueue_enqueue : renvoie un pointeur nul si ref vaut un pointeur nul. Tente
//    sinon d'enfiler ref dans la file de priorité associée à q ; renvoie un
//    pointeur nul en cas de dépassement de capacité ; renvoie sinon ref.
extern void *pqueue_enqueue(pqueue *q, const void *ref);

//  pqueue_dequeue : renvoie un pointeur nul si la file de priorité associée à q
//    est vide. Défile sinon la file et renvoie la référence qui était en tête
//    de la file avant le défilement.
extern void *pqueue_dequeue(pqueue *q);

//  pqueue_head : renvoie un pointeur nul si la file de priorité associée à q
//    est vide. Renvoie sinon la référence qui est en tête de la file.
extern void *pqueue_head(pqueue *q);

//  pqueue_is_empty : renvoie true ou false selon que la file de priorité
//    associée à q est vide ou non.
extern bool pqueue_is_empty(pqueue *q);

//  pqueue_length : renvoie la longueur de la file de priorité associée à q.
extern size_t pqueue_length(pqueue *q);

#include "pqueue_ip.h"

#endif
