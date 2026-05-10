//  bst_ip.h : précisions sur l'implantation du module bst.

//  Notation. n désigne la taille et h la hauteur de l'arbre binaire de
//    recherche associé au paramètre t (ou à *tptr).

//  bst_empty : temps Θ(1) ; espace Θ(1).

//  bst_dispose : temps Θ(n) ; espace O(h).

//  bst_add_endofpath : temps Ω(1) à O(h) ; espace O(h).

//  bst_remove_climbup_left : temps Ω(1) à O(h) ; espace O(h).

//  bst_search : temps Ω(1) à O(h) ; espace Θ(1).

//  bst_size : temps Θ(n) ; espace O(h).

//  bst_height : temps Θ(n) ; espace O(h).

//  bst_distance : temps Θ(n) ; espace O(h).

//  bst_number : temps ... ; espace ...

//  bst_rank : temps ... ; espace ...

//  bst_repr_graphic : temps Θ(n²) ; espace O(h).

//  bst_dft_infix_apply_context : temps ... ; espace ...
