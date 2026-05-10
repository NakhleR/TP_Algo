//  bt.h : partie interface d'un module pour la spécification ABIN du TDA ABin.

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#define __CH_VERSION_BT_H__ 202501L

#ifndef BT__H
#define BT__H

#include <stdio.h>
#include <stdlib.h>

//  struct bt, bt : type et nom de type d'un contrôleur regroupant les
//    informations permettant de gérer un arbre binaire.
typedef struct bt bt;

//  bt_comb_left, bt_comb_right, bt_random : tente d'allouer les ressources
//    nécessaires pour gérer un nouveau peigne gauche, un nouveau peigne droit,
//    un nouvel arbre binaire aléatoire, de taille n. Renvoie un pointeur nul en
//    cas de dépassement de capacité. Renvoie sinon un pointeur vers le
//    contrôleur associé à l'arbre binaire.
extern bt *bt_comb_left(size_t n);
extern bt *bt_comb_right(size_t n);
extern bt *bt_random(size_t n);

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    ou l'un de leurs paramètres de type bt * n'est pas l'adresse d'un objet
//    préalablement renvoyé par bt_comb_left, bt_comb_right ou bt_random et non
//    révoqué par bt_dispose depuis. Cette règle ne souffre que d'une seule
//    exception : bt_dispose tolère que la déréférence de son argument ait pour
//    valeur un pointeur nul.

//  bt_dispose : sans effet si *tptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de l'arbre binaire associé à *tptr puis
//    affecte à *tptr la valeur un pointeur nul.
extern void bt_dispose(bt **tptr);

//  bt_size, bt_height, bt_distance : renvoie la taille, la hauteur, la
//    distance de l'arbre binaire associé à t.
extern size_t bt_size(bt *t);
extern size_t bt_height(bt *t);
extern size_t bt_distance(bt *t);

//  bt_is_skinny, bt_is_comb_left, bt_is_comb_right : teste si l'arbre binaire
//    associé à t est filiforme, un peigne gauche, un peigne droit.
extern bool bt_is_skinny(bt *t);
extern bool bt_is_comb_left(bt *t);
extern bool bt_is_comb_right(bt *t);

//  bt_is_similar : teste si l'arbre binaire associé à t1 est semblable à celui
//    associé à t2.
extern bool bt_is_similar(bt *t1, bt *t2);

//  bt_repr_formal, bt_repr_lukas, bt_repr_subtree, bt_repr_graphic : affiche
//    sur la sortie standard la notation formelle, le mot de Łukasiewicz, le mot
//    des sous-arbres, une représentation graphique de l'arbre binaire associé à
//    t.
extern void bt_repr_formal(bt *t);
extern void bt_repr_lukas(bt *t);
extern void bt_repr_subtrees(bt *t);
extern void bt_repr_graphic(bt *t);

#include "bt_ip.h"

#endif
