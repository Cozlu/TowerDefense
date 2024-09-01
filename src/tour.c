#include "../include/tour.h"
#include "../include/constantes.h"
#include <stdio.h>


Gemme ajout_gemme_tour(Tour *tour, Gemme g){
    Gemme tmp = tour->g;
    tour->g = g;
    tour->contient_gemme = 1;
    tour->prochain_proj = time(NULL)*1000 + 2000;  // 2 secondes de délai
    return tmp;
}

Gemme retire_gemme_tour(Tour *tour){
    tour->contient_gemme = 0;
    return tour->g;
}

void nouvelle_tour(Tour *t){
    t->contient_gemme = 0;
    t->prochain_proj = 0;
}

int tour_a_gemme(const Tour *tour){
    return tour->contient_gemme;
}

int cout_tour(int num_tour){
    if(num_tour < 4) return 0;
    // 100 * 2^(num_tour - 4)
    return 100 * (1 << (num_tour - 4));
}
