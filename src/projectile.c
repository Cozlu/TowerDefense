#include "../include/constantes.h"
#include "../include/projectile.h"
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Convertit un nombre de degré en radiant
 * 
 * @param deg les degrés à convetir
 * @return double le résultat de la conversion
 */
static double deg_to_rad(double deg){
    return deg * PI/180;
}

/**
 * @brief Calcule les dégâts à appliquer au monstre
 * 
 * @param g gemme qui a tiré
 * @param m monstre ciblé
 * @return int les dégâts calculés
 */
static int calcul_degats(const Gemme *g, const Monstre *m){
    int degats;
    // constante * 2^niv * (1 - cos(teinte_g - teinte_m)/2)
    degats = CONSTANTE_DEGATS * (1 << g->niv) * (1 - cos(deg_to_rad(g->teinte - m->teinte))/2);
    if(!g->pur && rand()%10 == 0){
        return degats *2; // 1/10 de chance de doubler les dégats pour gemme mixe
    }
    return degats;
}

float distance_cible(const Projectile *proj){
    Monstre * cible = proj->cible;
    return dist_eucl(proj->x, proj->y, cible->x, cible->y);
}

/**
 * @brief Parcours la liste et met cible à NULL pour tous les projectiles qui cible *cible_mort
 * et déplace la cible des proj qui ciblait cible_bouge vers nouvelle_cible
 * 
 * @param tab_proj tableau des projectiles
 * @param cible_mort monstre mort
 * @param cible_bouge monstre qui a bougé dans la liste
 * @param nouvelle_cible nouvelle adresse du monstre qui a bougé
 * @return int nombre de cible mis à NULL (pour le debug)
 */
static int update_projectiles(TabProj *tab_proj, Monstre *cible_mort, Monstre *cible_bouge, Monstre *nouvelle_cible){
    Projectile *tmp = NULL;
    int nb_null = 0;
    for(int i = 0; i < tab_proj->proj.nb_elem; i++){
        tmp = (Projectile*) get_elem(&tab_proj->proj, i);
        if(tmp->cible == cible_mort){
            tmp->cible = NULL;
            nb_null++;
        }
        else if(tmp->cible == cible_bouge){
            tmp->cible = nouvelle_cible;
        }
    }
    return nb_null;
}

void suppr_monstre_et_proj(Monstre *cible_mort, TabProj *tab_proj, TabVagues *tab_vagues){
    Monstre *bouge = dernier_monstre(tab_vagues);
    int indice = (int) (cible_mort - (Monstre*) tab_vagues->vague.elems);
    suppr_monstre(tab_vagues, indice);
    update_projectiles(tab_proj, cible_mort, bouge, cible_mort);
}

/**
 * @brief Applique les dégâts à la cible, et met à jour les projectiles s'il meurt
 *
 * @param degats dégâts à infliger
 * @param cible monstre ciblé
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @return int les pv initiaux du monstre si le monstre est mort 0 sinon
 */
static int applique_degats(int degats, Monstre *cible, TabProj *tab_proj, TabVagues *tab_vagues){
    int degats_real = 0;
    if((degats_real = degats_sur_monstre(cible, degats))){ // monstre mort  
        suppr_monstre_et_proj(cible, tab_proj, tab_vagues);
        tab_proj->somme_mort++;
        tab_proj->somme_degats += degats_real;
        return cible->pv_init;
    }
    else{
        tab_proj->somme_degats += degats;
    }
    
    return 0;
}

/**
 * @brief Applique à tous les monstres qui sont à une distance plus petite que distance les dégâts de la gemme, 
 * multiplié par ratio_degats
 * 
 * @param g Gemme qui a tiré le projectile
 * @param m monstre au centre de l'explosion
 * @param distance rayon de l'explosion
 * @param ratio_degat pourcentage des dégâts que l'on applique aux autre
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @return int la somme des pv des monstre morts
 */
static int applique_degats_zone(const Gemme *g, Monstre *m, float distance, float ratio_degat, TabProj *tab_proj, TabVagues *tab_vagues){
    int degats;
    int somme_pv = 0;
    Monstre *cur;
    for(int i = tab_vagues->vague.nb_elem - 1; i >= 0 ; i--){
        cur = (Monstre *) get_elem(&tab_vagues->vague, i);
        if(m != cur && cur->est_apparu && distance_monstre(m, cur) <= distance){
            degats = calcul_degats(g, cur) * ratio_degat;
            somme_pv += applique_degats(degats, cur, tab_proj, tab_vagues);
        }
    }
    return somme_pv;
}

/**
 * @brief Applique l'effet combiné entre le résidu et l'élément de la gemme
 * 
 * @details on l'utilise si m->residu != element(gemme)
 * @param m Monstre auquel appliquer l'effet
 * @param g Gemme qui a fait l'attaque
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @param tab_anim tableau des animations
 * @return int la somme des pv initiaux des monstres morts
 */
static int applique_effet_combine(Monstre *m, const Gemme *g, TabProj *tab_proj, TabVagues *tab_vagues, TabAnimation *tab_anim){
    Effet *effet = &m->effet;
    Effet_Type elem_combine = element_gemme(g) + effet->residu;
    Animation new_anim;
    // si déjà un effet combine on ne fait rien
    if(effet->element != NO_EFFECT) return NO_EFFECT;
    
    switch (elem_combine){
        case VAPO:
            effet->residu = NO_EFFECT;
            effet->element = VAPO;
            effet->fin_effet = time(NULL) + 10;
            m->vitesse =  m->vitesse_init*4/5;
            init_animation(&new_anim, VAPO, m->x, m->y, 3.5);
            add_elem(tab_anim, &new_anim);
            return applique_degats_zone(g, m, 3.5, 0.05 , tab_proj, tab_vagues); 
        case ENRACIN:
            effet->residu = NO_EFFECT;
            effet->fin_effet = time(NULL) + 3;
            effet->element = ENRACIN;
            m->vitesse = 0;
            return 0;
        case COMBUST:
            effet->residu = NO_EFFECT;
            effet->element = COMBUST;
            effet->fin_effet = time(NULL) + 1;
            // on applique 2 fois les dégats car on a déjà appliqué une fois les dégâts avant cette fonction
            if(applique_degats(calcul_degats(g, m) * 2, m, tab_proj, tab_vagues))
                return m->pv_init;
            return 0;
        default:
            return 0;
    }
}

/**
 * @brief Applique l'effet associé au résidu du monstre
 * 
 * @details on l'utilise si m->residu == element(gemme)
 * @param m Monstre auquel appliquer l'effet
 * @param g Gemme qui a fait l'attaque
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @param tab_anim tableau des animations
 * @return int la somme des pv initiaux des monstres morts
 */
static int applique_effet_special(Monstre *m, const Gemme *g, TabProj *tab_proj, TabVagues *tab_vagues, TabAnimation *tab_anim){
    Effet *effet = &m->effet;
    Animation new_anim;
    switch(effet->residu){
        case PYRO:
            effet->element = PYRO;
            effet->fin_effet = time(NULL) + 1;
            init_animation(&new_anim, PYRO, m->x, m->y, 2);
            add_elem(tab_anim, &new_anim);
            return applique_degats_zone(g, m, 2, 0.15 , tab_proj, tab_vagues);
        case DENDRO:
            effet->element = DENDRO;
            effet->fin_effet = time(NULL) + 10;
            effet->prochaine_maj = time(NULL)*1000 + 500;
            effet->degats = calcul_degats(g, m) * 0.025;
            return 0;
        case HYDRO:
            effet->element = HYDRO;
            effet->fin_effet = time(NULL) + 10;
            m->vitesse = m->vitesse_init * 2/3;
            return 0;
        default:
            return 0;
    }
}

/**
 * @brief Applique l'effet de la gemme au monstre
 * 
 * @param m Monstre auquel appliquer l'effet
 * @param g Gemme qui a fait l'attaque
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @param tab_anim tableau des animations
 * @return int la somme des pv initiaux des monstres morts
 */
static int applique_effet(Monstre *m, const Gemme *g, TabProj *tab_proj, TabVagues *tab_vagues, TabAnimation *tab_anim){
    Element elem = element_gemme(g);
    // pas encore de résidu
    if(!m->effet.residu){
        m->effet.residu = elem;
        return 0;
    }
    // effets spéciaux
    if(m->effet.residu == elem){
        return applique_effet_special(m, g, tab_proj, tab_vagues, tab_anim);
    }
    // effet combiné
    return applique_effet_combine(m, g, tab_proj, tab_vagues, tab_anim);
}

/**
 * @brief Inflige les dégâts au monstre, et est supprimer de la liste
 * 
 * @details si le monstre meurt, tous les projectiles qui le visent disparaissent
 * @param tab_proj tableau des projectiles
 * @param indice_proj indice du projectile dans le tableau
 * @param tab_vagues tableau des monstres
 * @param tab_anim tableau des animations
 * @return int somme des pv des monstre tués
 */
static int explose_projectile(TabProj *tab_proj, int indice_proj, TabVagues *tab_vagues, TabAnimation *tab_anim){
    Projectile *proj = (Projectile *) get_elem(&tab_proj->proj, indice_proj);
    Gemme *g = &proj->gemme;
    Monstre *m = proj->cible;
    int sum_pv = applique_degats(calcul_degats(g, m), m, tab_proj, tab_vagues);
    if(g->pur && sum_pv == 0){
        sum_pv += applique_effet(m, g, tab_proj, tab_vagues, tab_anim);
    }
    proj->cible = NULL;
    return sum_pv;
}

/**
 * @brief Avance le projectile dans la direction du monstre
 * 
 * @param proj Projectile à avancer
 * @return int 1 si le monstre est arrivé sur le monstre 0 sinon
 */
static int avance_projectile(Projectile *proj){
    float dist = distance_cible(proj);
    Monstre * cible = proj->cible;
    if(dist <= 3*DISTANCE_PAR_MAJ){
        proj->x = cible->x;
        proj->y = cible->y;
        return 1; // projectile doit exploser
    }
    proj->x += 3*DISTANCE_PAR_MAJ*(cible->x - proj->x)/dist;
    proj->y += 3*DISTANCE_PAR_MAJ*(cible->y - proj->y)/dist;
    if(proj->x < 0 || proj->x > TAILLE_CASE*NB_COLS ||  proj->y < 0 || proj->y > TAILLE_CASE*NB_LIGNES){
        proj->cible = NULL; // protection en cas de bug (ne devrait pas arriver)
    }
    return 0;
}

int nouveau_projectile(TabProj *tab_proj, const Tour* tour, int li, int col, Monstre *monstr){
    Projectile proj;
    proj.cible = monstr;
    proj.gemme = tour->g;
    case_vers_coord_centre(li, col, &proj.x, &proj.y);
    return add_elem(&tab_proj->proj, &proj);
}


/**
 * @brief renvoie la distance en nb_cases entre le centre du monstre et le centre de la tour
 * 
 * @param li ligne de la tour
 * @param col colonne de la tour
 * @param monstre pointeur vers le monstre
 * @return float distance en nb cases
 */
static float distance_tour_monstre(int li, int col, const Monstre *m){
    float x,y;
    case_vers_coord_centre(li, col, &x, &y);
    return dist_eucl_case(x, y, m->x, m->y);
}


/**
 * @brief cherche la prochaine cible de la tour
 * @param tour tour
 * @param tab_vague tableau des monstres
 * 
 * @return renvois le monstre le plus proche que la tour peut cibler \n
 * NULL si aucun monstre n'est à porter
 */
static Monstre *trouve_cible(const Tour *tour, int li, int col, const TabVagues *tab_vague){
    float distance_tmp;
    float detection = distance_detection_gemme(tour->g.niv);
    float distance_min = detection;
    Monstre *monstre_proche = NULL, *cur = NULL;
    
    for(int i = 0; i < tab_vague->vague.nb_elem; i++){
        cur = (Monstre *) get_elem(&tab_vague->vague, i);
        if(cur->est_apparu)
            if((distance_tmp = distance_tour_monstre(li, col, cur)) <= detection){
                if(distance_min > distance_tmp){
                    monstre_proche = cur;
                    distance_min = distance_tmp;
                }
            }
    }
    return monstre_proche;
}

int all_nouveaux_projectiles(TabProj *tab_proj, Terrain *terrain, const TabVagues *tab_vague, struct timespec cur_time){
    Tour *tour = NULL;
    Monstre *m = NULL;
    int nb_proj = 0;
    time_ms cur_time_ms = cur_time.tv_sec*1000 + cur_time.tv_nsec/1.0E6;
    for(int i = 0; i < NB_LIGNES; i++){
        for(int j = 0; j < NB_COLS; j++){
            if(terrain->carte[i][j].type != TOUR)
                continue;
            tour = &terrain->carte[i][j].donnee.tour;
            if(tour->contient_gemme && cur_time_ms > tour->prochain_proj){
                if((m = trouve_cible(tour, i, j, tab_vague))){
                    if(nouveau_projectile(tab_proj, tour, i, j, m))
                        return -1;  // problème d'allocation
                    tour->prochain_proj = cur_time_ms + delai_tir_gemme(tour->g.niv);
                    nb_proj++;
                }
            } 
        }

    }
    return nb_proj;
}

void decaler_cibles_proj(TabProj *tab_proj, size_t decalage){
    Projectile *proj;
    for(int i = 0; i < tab_proj->proj.nb_elem; i++){
        proj = (Projectile *) get_elem(&tab_proj->proj, i);
        proj->cible = (Monstre*) ((char*) proj->cible + decalage);
    }
}

/**
 * @brief Fonction de filtrage des projectiles selon s'ils ont des cibles ou non
 * 
 * @param p projectile à filtrer
 * @return int 0 si il n'y a pas de cible et 1 sinon 
 */
static int filtre_proj(const void *p){
    const Projectile *proj = (Projectile*) p;
    return proj->cible == NULL;
}

int avance_tab_proj(TabProj *tab_proj, TabVagues *tab_vagues, TabAnimation *tab_anim){
    int sum_pv = 0;
    Projectile *proj;
    for(int i = 0; i < tab_proj->proj.nb_elem; i++){
        proj = (Projectile *) get_elem(&tab_proj->proj, i);
        if(proj->cible != NULL && avance_projectile(proj)){
            sum_pv += explose_projectile(tab_proj, i, tab_vagues, tab_anim);
        }
    }
    filter_elem(&tab_proj->proj, &filtre_proj);
    return sum_pv;
}

int init_tab_proj(TabProj *tab_proj){
    tab_proj->somme_degats = 0;
    tab_proj->somme_mort = 0;
    if(init_GenericTab(&tab_proj->proj, sizeof(Projectile), 100)){
        return 1;
    }
    return 0;
}

void libere_tab_projectile(TabProj *tab_proj){
    if(tab_proj){
        free_GenericTab(&(tab_proj->proj));
    }
}
