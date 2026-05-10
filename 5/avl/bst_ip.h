//  bst_ip.h : précisions sur l'implantation du module bst (AVL).

//  Notation. n désigne la taille et h la hauteur de l'arbre AVL associé au
//    paramètre t (ou à *tptr). Pour un arbre AVL, h = O(log n).

//  bst_empty : temps Θ(1) ; espace Θ(1).

//  bst_dispose : temps Θ(n) ; espace O(h) = O(log n).

//  bst_add_endofpath : temps Ω(1) à O(h) = O(log n) ; espace O(h) = O(log n).

//  bst_remove_climbup_left : temps Ω(1) à O(h) = O(log n) ;
//    espace O(h) = O(log n).

//  bst_search : temps Ω(1) à O(h) = O(log n) ; espace Θ(1).

//  bst_size : temps Θ(n) ; espace O(h) = O(log n).

//  bst_height : temps Θ(1) ; espace Θ(1).

//  bst_distance : temps Θ(n) ; espace O(h) = O(log n).

//  bst_number : temps ... ; espace ...

//  bst_rank : temps ... ; espace ...

//  bst_repr_graphic : temps Θ(n) ; espace O(h) = O(log n).

//  bst_dft_infix_apply_context : temps ... ; espace ...
