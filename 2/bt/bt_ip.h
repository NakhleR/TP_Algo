//  bt_ip.h : précisions sur l'implantation du module bt.

//  Notation. Pour bt_comb_left, bt_comb_right et bt_random, n désigne la
//    valeur du paramètre. Pour les autres fonctions, n désigne la taille et h
//    la hauteur de l'arbre binaire associé au paramètre t (ou à *tptr). Pour
//    bt_is_similar, n1, h1 (resp. n2, h2) désignent la taille et la hauteur
//    de l'arbre associé à t1 (resp. t2).

//  bt_comb_left : temps Θ(n) ; espace Θ(n).

//  bt_comb_right : temps Θ(n) ; espace Θ(n).

//  bt_random : temps Θ(n) ; espace O(n).

//  bt_dispose : temps Θ(n) ; espace O(h).

//  bt_size, bt_height, bt_distance : temps Θ(n) ; espace O(h).

//  bt_is_skinny : temps Ω(1) à O(n) ; espace O(h).

//  bt_is_comb_left, bt_is_comb_right : temps Ω(1) à O(h) ; espace O(h).

//  bt_is_similar : temps Ω(1) à O(min(n1, n2)) ; espace O(min(h1, h2)).

//  bt_repr_formal : temps Θ(n) ; espace O(h).

//  bt_repr_lukas : temps Θ(n) ; espace O(h).

//  bt_repr_subtrees : temps Θ(n) ; espace O(h).

//  bt_repr_graphic : temps Θ(n) ; espace O(h).
