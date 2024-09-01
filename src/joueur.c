#include "../include/joueur.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init_mana(Mana *m){
    m->niv = 1;
    m->qte_max = 2000;
    m->reserve = 150;
    m->cout = 1;
}

void init_joueur(Joueur *j){
    j->contient_gemme = init_Bittab();
    init_mana(&(j->jauge));
}

int gemme_libre(const Joueur *j){
    for(int i = 0; i < GEMME_INV_MAX; i++){
        if(!get_bit(&j->contient_gemme, i)){
            return i;
        }
    }
    return -1;
}

int cout_agrandir_mana(int qte_max){
    return qte_max/4;
}

/**
 * @brief Vérifie si l'indice de la gemme est correcte
 * 
 * @param indice_g indice à vérifier
 */
static int indice_gemme_correct(int indice_g){
    return ( 0 <= indice_g  && indice_g <= GEMME_INV_MAX);
}

int joueur_creer_gemme(Joueur *j, int niv){
    int i;
    // conditions où la création est impossible
    if(niv < 0 || niv >= 25) return 1;
    if(j->jauge.reserve < prix_gemme(niv)) return 1;
    if((i = gemme_libre(j)) < 0) return 1;
    j->inventaire[i] = random_gemme(niv); 
    set_bit(&j->contient_gemme, i, 1);
    j->jauge.reserve -= prix_gemme(niv);
    return 0;
}

int joueur_bouger_gemme(Joueur *j, int gem1, int gem2){
    Gemme tmp;
    int tmp_contient;
    if(!indice_gemme_correct(gem1) || !indice_gemme_correct(gem2)) return 1;
    // échange de gemme
    tmp = j->inventaire[gem1];
    j->inventaire[gem1] = j->inventaire[gem2];
    j->inventaire[gem2] = tmp;

    // échange de bit
    tmp_contient = get_bit(&j->contient_gemme, gem1);
    set_bit(&j->contient_gemme, gem1, get_bit(&j->contient_gemme, gem2));
    set_bit(&j->contient_gemme, gem2, tmp_contient);
    return 0;
}

int joueur_fusionner_gemmes(Joueur *j, int gem1, int gem2){
    // conditions où la fusion est impossible
    if(j->jauge.reserve < COUT_FUSION_GEMME) return 1;
    if(gem1 == gem2) return 1;
    if(!indice_gemme_correct(gem1) || !indice_gemme_correct(gem2)) return 1;
    if(!get_bit(&j->contient_gemme, gem1) && !get_bit(&j->contient_gemme, gem2)) return 1;    
    if(!get_bit(&j->contient_gemme, gem1) || !get_bit(&j->contient_gemme, gem2)){ 
        return joueur_bouger_gemme(j, gem1, gem2);
    }
    if(!peuvent_fusionner(j->inventaire + gem1, j->inventaire + gem2)) return 1;
    
    // on fusionne les deux
    j->inventaire[gem1] = fusion_gemmes(j->inventaire + gem1, j->inventaire + gem2);
    set_bit(&j->contient_gemme, gem2, 0);
    j->jauge.reserve -= COUT_FUSION_GEMME;
    return 0;
}

int joueur_agrandir_reserve(Joueur *j){
    float prix = cout_agrandir_mana(j->jauge.qte_max);
    
    // condition où l'agrandissement est impossible
    if(j->jauge.reserve < prix || j->jauge.niv >= 200) return 1;

    j->jauge.reserve -= prix;
    j->jauge.niv++;
    j->jauge.qte_max *= 1.4;
    j->jauge.cout *= 1.3;
    return 0;
}

int joueur_batire_tour(Joueur *j, Terrain *terrain, const Coord *click){
    // conditions où la création est impossible
    long cout_mana;
    Case *c;
    if(!case_est_libre(terrain, click)) return 1;
    cout_mana = cout_tour(terrain->nb_tours + 1);
    if(cout_mana >= j->jauge.reserve) return 1; // pas assez de mana

    c = &terrain->carte[click->li][click->col];
    c->type = TOUR;

    nouvelle_tour(&c->donnee.tour);
    terrain->nb_tours++;
    if (terrain->nb_tours > 3) j->jauge.reserve -= cout_mana;
    
    return 0;
}

int joueur_poser_gemme(Joueur *j, int indice_g, Terrain *terrain, const Coord *click){
    Tour *tour;
    // conditions où la pose est impossible
    if(!indice_gemme_correct(indice_g)) return 1;
    if(!(tour = tour_case(terrain, click->li, click->col))) return 1;
    if(!get_bit(&j->contient_gemme, indice_g)) return 1;

    if (tour_a_gemme(tour)) { // on échange les deux gemmes
        j->inventaire[indice_g] = ajout_gemme_tour(tour, j->inventaire[indice_g]);

    } else {
        // on ajoute juste la gemme
        ajout_gemme_tour(tour, j->inventaire[indice_g]);
        set_bit(&j->contient_gemme, indice_g, 0);
    }
    return 0;
}

int joueur_retirer_gemme(Joueur *j, int indice_g, Terrain *terrain, const Coord *tour_sel){
    Tour *tour;
    // conditions où la pose est impossible
    if(!indice_gemme_correct(indice_g)) return 1;
    if(!(tour = tour_case(terrain, tour_sel->li, tour_sel->col))) return 1;

    if (get_bit(&j->contient_gemme, indice_g)) { // on echange les deux gemmes
        j->inventaire[indice_g] = ajout_gemme_tour(tour, j->inventaire[indice_g]);
    } else {
        // on retire juste la gemme
        j->inventaire[indice_g] = retire_gemme_tour(tour);
        set_bit(&j->contient_gemme, indice_g, 1);
    }
    return 0;
}

void ajout_mana_monstre(Joueur *j, float pv_init){
    //  =  1.3 ^niv * pv_init *10%
    float ajout = j->jauge.cout * pv_init * 0.1;
    j->jauge.reserve = MIN(j->jauge.reserve + ajout, j->jauge.qte_max);
}

int retirer_mana_monstre(Joueur *j, float pv_init){
    // = 1.3^niv * pv_init * 15%
    j->jauge.reserve -= j->jauge.cout * pv_init * 0.15;
    return j->jauge.reserve >= 0;
}
