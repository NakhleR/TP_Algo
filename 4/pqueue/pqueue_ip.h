//  pqueue_ip.h : précisions sur l'implantation du module pqueue.

//  Notation. n désigne la longueur de la file de priorité associée à q (ou
//    à *qptr).

//  pqueue_empty : temps Θ(1) ; espace Θ(1).
//  pqueue_dispose : temps Θ(1) ; espace Θ(1).
//  pqueue_enqueue : temps amorti Ω(1) à O(log n) ; espace Θ(1).
//  pqueue_dequeue : temps Ω(1) à O(log n) ; espace Θ(1).
//  pqueue_head : temps Θ(1) ; espace Θ(1).
//  pqueue_is_empty : temps Θ(1) ; espace Θ(1).
//  pqueue_length : temps Θ(1) ; espace Θ(1).
