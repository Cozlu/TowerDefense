#include "../include/monstre.h"
#include <stdio.h>
#include <stdlib.h>

Monstre init_monstre(int teinte, float vitesse, float pv, float x, float y) {
    Monstre monstre;
    monstre.teinte = teinte;
    monstre.est_apparu = 0;
    monstre.vitesse = vitesse;
    monstre.vitesse_init = vitesse;
    monstre.pv = pv;
    monstre.pv_init = pv;
    monstre.x = x;
    monstre.y = y;
    init_effet_vide(&monstre.effet);
    return monstre;
}

int init_tab_vagues(TabVagues * tab_vagues) {
    if(init_GenericTab(&tab_vagues->vague, sizeof(Monstre), 100))
        return 1;
    tab_vagues->num_vague = 0;
    tab_vagues->pv_vague = 1;
    return 0;
}

int ajout_monstre(TabVagues * tab_vagues, Monstre monstre) {
    return add_elem(&tab_vagues->vague, &monstre);
}

void suppr_monstre(TabVagues * tab_vagues, int i) {
    remove_elem(&tab_vagues->vague, i);
}

int degats_sur_monstre(Monstre *m, int degats){
    int degats_reel;
    m->pv -= degats;
    if(m->pv <= 0){
        degats_reel = m->pv + degats;
        return degats_reel;
    }
    return 0;
}

Monstre *dernier_monstre(const TabVagues *tab_vagues){
    return get_last_elem(&tab_vagues->vague);
}

void libere_tab_vagues(TabVagues * tab_vagues) {
    if (tab_vagues) {
        free_GenericTab(&tab_vagues->vague);
    }
}

float distance_monstre(const Monstre *m1, const Monstre *m2) {
    return dist_eucl_case(m1->x, m1->y, m2->x, m2->y);
}

/**
 * @brief Tire la distance que doit parcourir le monstre
 * 
 * @param m monstre pour tirer la distance
 * @return float distance à parcourir
 */
static float tire_avance(Monstre m) {
    float min = 0.9 * m.vitesse * DISTANCE_PAR_MAJ;
    float max = 1.1 * m.vitesse * DISTANCE_PAR_MAJ;
    float avance = min + (max - min)* rand()/(double) RAND_MAX;
    return avance;
}

int avancer_monstre(Monstre * m, const Terrain *terrain) {
    Coord courante, nid = terrain->nid;
    float x_case,y_case;
    float avance = tire_avance(*m);
    float diff_x, diff_y;
    coord_vers_case(m->x, m->y, &courante);
    
    // on cherche la direction du monstre
    if (!m->est_apparu) {
        if(case_egal(&nid, &courante))
            // si le monstre atteint le nid alors il est apparu
            m->est_apparu = 1;
    } else if (m->dir != chemin_vers_dir(terrain, courante)){
        case_vers_coord_centre(courante.li, courante.col, &x_case, &y_case);
        diff_x = abs(x_case - m->x);
        diff_y = abs(y_case - m->y);
        // si le monstre a dépassé le milieu on change la direction
        if(diff_x <= avance && diff_y <= avance){
            m->x = x_case;
            m->y = y_case;
            m->dir = chemin_vers_dir(terrain, courante);
            avance -= diff_x? diff_y : diff_x; // on n'avance que dans des direction cardinales donc diff_x = 0 ou diff_y = 0
        }
    }
    
    // le monstre touche le camp
    if (case_egal(&courante, &terrain->camp)) {
        case_vers_coord_centre(nid.li, nid.col, &(m->x), &(m->y));
        return 1;
    }
    // on avance le monstre
    m->y += dir_li(m->dir)*avance;
    m->x += dir_col(m->dir)*avance;
    return 0;
}

int maj_effet_monstre(Monstre *m, struct timespec cur_time){
    time_ms cur_time_ms = cur_time.tv_sec*1000 + cur_time.tv_nsec/1.0E6;
    switch(m->effet.element){
        case DENDRO:
            if(cur_time_ms >= m->effet.prochaine_maj){
                m->effet.prochaine_maj += 500;   // intervalle de 500 ms
                return degats_sur_monstre(m, m->effet.degats);
            }
            if(cur_time.tv_sec >= m->effet.fin_effet){
                m->effet.element = NO_EFFECT;
            }
            break;
        case HYDRO: // même comportements pour HYDRO, VAPO et ENRACIN
        case VAPO:
        case ENRACIN:
            if(cur_time.tv_sec >= m->effet.fin_effet){
                m->vitesse = m->vitesse_init;
                m->effet.element = NO_EFFECT;
            }
            break;
        default:
            if(cur_time.tv_sec >= m->effet.fin_effet){
                m->effet.element = NO_EFFECT;
            }
            break;
    }
    return 0;
}


/**
 * @brief Créée une vague de monstres
 * 
 * @param tab_vagues pointeur vers le tableau à modifier
 * @param nb_monstres nombre de monstres à créer
 * @param vitesse vitesse des monstres de la vague
 * @param type_vague type de la vague
 * @param terrain terrain du jeu
 * @return int 0 si il n'y a pas eu d'erreur d'allocation et 1 sinon
 */
static int creer_vague(TabVagues * tab_vagues, int nb_monstres, float vitesse, TypeVague type_vague, Terrain *terrain) {
    float x, y, pv;
    Monstre m;
    Coord nid = terrain->nid;
    Direction dir = chemin_vers_dir(terrain, nid);
    int dli = dir_li(inverse_dir(dir)),  dcol = dir_col(inverse_dir(dir));
    
    case_vers_coord_centre(nid.li, nid.col, &x, &y);
    if (type_vague == BOSS)
        pv = 12*CONSTANTE_PV*tab_vagues->pv_vague;  // 12 * C * 1.2^num_vague
    else
        pv = CONSTANTE_PV*tab_vagues->pv_vague;    // C * 1.2^num_vague
    for(int i = 0; i < nb_monstres; i++) {
        m = init_monstre(rand() % 359, vitesse, pv, x, y);
        m.dir = dir;
        if (ajout_monstre(tab_vagues, m) == 1)
            return 1; // erreur d'allocation
    
        // on place les monstres de la vague en file derrière la case du nid
        y += dli*TAILLE_CASE;
        x += dcol*TAILLE_CASE;
    }
    return 0;
}

/**
 * @brief Tire aléatoirement la vague à créer
 * 
 * @return TypeVague type de la vague tirée
 */
static TypeVague tirage_vague() {
    int proba = rand() % 100;
    if (proba < 10)
        return BOSS;
    if (proba < 20)
        return AGILE;
    if (proba < 40)
        return FOULE;
    return NORMALE;
}

int vague_aleatoire(TabVagues *tab_vagues, Terrain *terrain) {
    int nb_monstres;
    float vitesse;
    TypeVague tirage = tirage_vague();
    if(tab_vagues->num_vague <= 5){ // on ne créée pas de vague boss avant la 6ème vague
        while(tirage == BOSS) 
            tirage = tirage_vague(); // dans ce cas on tire de nouveau une vague
    }
    switch(tirage) {
        case NORMALE : nb_monstres = 12; vitesse = 1; break;
        case FOULE : nb_monstres = 24; vitesse = 1; break;
        case AGILE : nb_monstres = 12; vitesse = 2; break;
        case BOSS : nb_monstres = 2; vitesse = 1; break;
    }

    tab_vagues->num_vague += 1;
    tab_vagues->pv_vague *= 1.2;
    return creer_vague(tab_vagues, nb_monstres, vitesse, tirage, terrain);
}
