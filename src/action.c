#include <stdlib.h>
#include <stdio.h>
#include "../include/action.h" 

int coord_vers_case_inv(int mouse_x, int mouse_y){
    return (mouse_x - (WIN_LARGEUR - 255))/TAILLE_GEMME_INV + (mouse_y - 250)/TAILLE_GEMME_INV *3;
}

Zones_Clic zone_souris(int mouse_x, int mouse_y) {
    if (mouse_x < TAILLE_CASE*NB_COLS && mouse_y < TAILLE_CASE*NB_LIGNES)
        return TERRAIN;
    if (mouse_x >= (WIN_LARGEUR - 255) && mouse_x <= (WIN_LARGEUR - 255 + 80*3) && mouse_y >= 250 && mouse_y <= (250 + 80*9))
        return INVENTAIRE;
    if (mouse_y >= (WIN_HAUTEUR - 80) && mouse_y <= (WIN_HAUTEUR - 20)) {
        if (mouse_x >= 660 && mouse_x <= (660 + 150))
            return AGGRANDIR_RESERVE;
        if (mouse_x >= 820 && mouse_x <= (820 + 250))
            return VAGUE_SUIVANTE;
    }
    if (mouse_x >= WIN_LARGEUR - 50 && mouse_y <= 50)
        return CROIX;
    return 0;
}

/**
 * @brief Effectue toutes les actions du joueur dans l'inventaire
 * 
 * @param jeu état courant du jeu
 * @param mouse_x coordonnée x du clic de la souris
 * @param mouse_y coordonnée y du clic de la souris
 */
static void action_inventaire(Jeu *jeu, int mouse_x, int mouse_y) {
    int case_inv = coord_vers_case_inv(mouse_x, mouse_y);
    if(get_bit(&jeu->joueur.contient_gemme, case_inv)){ // Si il y a une gemme à cette case on la sélectionne
        jeu->gem_sel = case_inv;
    }
    jeu->tour_sel.col = -1; // déselection de la tour
    jeu->tour_sel.li = -1;
}

/**
 * @brief Effectue toutes les actions du joueur sur le terrain : 
 * - poser/sélectionner une tour 
 * - placer les gemmes
 * 
 * @param jeu le jeu à modifier selon l'action
 * @param mouse_x coordonnée x du clic de la souris
 * @param mouse_y coordonnée y du clic de la souris
 */
static void action_terrain(Jeu *jeu, int mouse_x, int mouse_y) {
    Coord click;
    int indice_g = -1;
    coord_vers_case(mouse_x, mouse_y, &click);
    if (case_est_libre(&(jeu->terrain), &click))  // créer tour
        joueur_batire_tour(&(jeu->joueur), &(jeu->terrain), &click);
    else if (jeu->terrain.carte[click.li][click.col].type == TOUR ){  
        /*Si la case sur laquelle on clique est une tour et qu'elle possède une gemme, on retire la gemme*/
        if(case_egal(&click, &jeu->tour_sel) && tour_a_gemme(tour_case(&jeu->terrain, click.li, click.col))){ 
            indice_g = gemme_libre(&jeu->joueur);
            joueur_retirer_gemme(&jeu->joueur, indice_g, &jeu->terrain, &jeu->tour_sel);
            jeu->gem_sel = indice_g;
        } else if (case_egal(&click, &jeu->tour_sel)) { // déselectionner tour
            jeu->tour_sel.li = -1;
            jeu->tour_sel.col = -1;
        } else // sélectionner tour
            jeu->tour_sel = click;
    }
}

int actions_pressed(Jeu *jeu, int mouse_x, int mouse_y, struct timespec cur_time) {
    switch(zone_souris(mouse_x, mouse_y)) {
        case TERRAIN : action_terrain(jeu, mouse_x, mouse_y); break;
        case INVENTAIRE : action_inventaire(jeu, mouse_x, mouse_y); break;
        case AGGRANDIR_RESERVE : joueur_agrandir_reserve(&(jeu->joueur)); break;
        case VAGUE_SUIVANTE : 
            if(vague_suivante(jeu, cur_time))
                return 1; // erreur d'allocation
            break;
        case CROIX : 
            return -1; // quitter le jeu
        default: // deselectionner
            jeu->gem_sel = -1;
            jeu->tour_sel.col = -1;
            jeu->tour_sel.li = -1; 
            break; 
    }
    return 0;
}


int actions_released(Jeu *jeu, int mouse_x, int mouse_y, struct timespec cur_time){
    Coord click;
    int i;
    if(jeu->gem_sel == -1) return 0; // si on n'a pas de gemme sélectionnée on ne fait rien
    switch(zone_souris(mouse_x, mouse_y)){
        case TERRAIN :
            coord_vers_case(mouse_x, mouse_y, &click);
            joueur_poser_gemme(&jeu->joueur, jeu->gem_sel, &jeu->terrain, &click);
            break;
        case INVENTAIRE:
            i = coord_vers_case_inv(mouse_x, mouse_y);
            joueur_fusionner_gemmes(&jeu->joueur, i, jeu->gem_sel);
            jeu->gem_sel = -1;
            break;
        default: break; // nothing
    }
    jeu->gem_sel = -1;
    return 0;
}
