#include "../include/jeu.h"
#include "../include/action.h"
#include <MLV/MLV_all.h>
#include <time.h>
#include <stdio.h>

int init_jeu(Jeu * jeu) {
    jeu->gem_sel = -1;
    jeu->tour_sel.col = -1;
    jeu->tour_sel.li = -1;
    jeu->prochaine_vague = time(NULL);
    jeu->terrain = init_terrain();
    init_joueur(&(jeu->joueur));
    if (init_tab_vagues(&(jeu->vagues)) || init_tab_proj(&(jeu->projectiles)) 
           || init_GenericTab(&jeu->animations, sizeof(Animation), 20))
        return 1;
    jeu->vagues.vague.name = "tab_vagues";
    jeu->projectiles.proj.name = "tab_proj";
    jeu->animations.name = "animations";
    return 0;
}

void libere_jeu(Jeu *jeu){
    libere_tab_projectile(&(jeu->projectiles));
    libere_tab_vagues(&(jeu->vagues));
    free_GenericTab(&jeu->animations);
}

/**
 * @brief Vérifie si le caractère est un chiffre
 * 
 * @param c le caractère à tester
 * @return int 1 si cd'est un chiffre et 0 sinon
 */
static int is_int(char c){
    return ('0' <= c && c <= '9'); 
}

/**
 * @brief Convertit la chaîne de caractère en entier positif
 * 
 * @param s chaîne à convertir
 * @param n adresse où mettre le résultat
 * @return int 1 si c'était possible 0 sinon
 */
static int str_to_int(const char *s, int *n){
    int tmp = 0;
    int puissance = 1; /**stocke la puissance de 10 dans laquelle on se trouve */
    for(int i = strlen(s) - 1; i >= 0; i--){
        if(!is_int(s[i])){
            return 0; 
        } /* si un des caractère n'est pas un chiffre*/
            
        tmp += (int)(s[i] - '0') * puissance;
        puissance *= 10;
    }
    *n = tmp;
    return 1;
}


/**
 * @brief Passe à la vague suivante
 * 
 * @param vagues vagues de monstre
 * @param jauge la jauge de manan du joueur
 * @param tab_proj tableau des projectiles
 * @param terrain le terrain du jeu
 * @param temps_restant le temps en secondes jusqu'à la prochaine vague théorique
 * @return 1 si il y a une erreur d'allocation et 0 sinon
 */
static int vague_suivante_aux(TabVagues *vagues, Mana *jauge, TabProj *tab_proj, Terrain *terrain, time_t temps_restant) {
    float ajout_reserve;
    const void * const old_vague = vagues->vague.elems; // a ne pas utiliser
    if (vague_aleatoire(vagues, terrain)) {
        return 1; //erreur d'allocation
    }
    if(vagues->vague.elems != old_vague){ // s'il y a eu un realloc
        size_t decalage = (char *)vagues->vague.elems - (char *)old_vague;
        decaler_cibles_proj(tab_proj, decalage);
    }
    ajout_reserve = temps_restant*(jauge->qte_max)/100;
    if ((jauge->reserve + ajout_reserve) > jauge->qte_max)
        ajout_reserve = jauge->qte_max - jauge->reserve;
    jauge->reserve += ajout_reserve;
    return 0;
}

int vague_suivante(Jeu *jeu, struct timespec cur_time){
    int temps_restant = jeu->prochaine_vague >= cur_time.tv_sec? jeu->prochaine_vague - cur_time.tv_sec: 0;
    jeu->prochaine_vague = cur_time.tv_sec + TEMPS_ENTRE_VAGUE;
    jeu->tour_sel.col = -1;
    jeu->tour_sel.li = -1;
    return vague_suivante_aux(&jeu->vagues, &jeu->joueur.jauge, &jeu->projectiles, &jeu->terrain, temps_restant);
}

/**
 * @brief Avance les monstres et met à jour leur effet
 *
 * @param jeu état courant du jeu
 * @param cur_time le temps courant
 */
static void maj_monstres(Jeu *jeu, struct timespec cur_time){
    Monstre *cur = NULL;
    for(int i = 0; i < jeu->vagues.vague.nb_elem; i++) {
        cur = (Monstre *)get_elem(&jeu->vagues.vague, i);
        if (avancer_monstre(cur, &jeu->terrain)) // monstre a passer le camp
            retirer_mana_monstre(&jeu->joueur, cur->pv_init);
        if(maj_effet_monstre(cur, cur_time)){   // monstre mort
            ajout_mana_monstre(&jeu->joueur, cur->pv_init);
            suppr_monstre_et_proj(cur, &jeu->projectiles, &jeu->vagues);
        }
    }
}

/**
 * @brief Avance les projectiles et ajoute le mana au joueur
 *
 * @param jeu état courant du jeu
 * @param cur_time le temps courant
 */
static int maj_projectiles(Jeu *jeu, struct timespec cur_time){
    int sum_pv = avance_tab_proj(&jeu->projectiles, &jeu->vagues, &jeu->animations);
    if(sum_pv > 0){
        ajout_mana_monstre(&jeu->joueur, sum_pv);
    }
    if(all_nouveaux_projectiles(&(jeu->projectiles), &(jeu->terrain), &(jeu->vagues), cur_time) < 0){
        return 1; // erreur d'allocation
    }
    return 0;
}

/**
 * @brief Met à jour les animations du jeu
 *
 * @param jeu état courant du jeu
 * @param cur_time le temps courant
 */
static int maj_animations(Jeu *jeu, struct timespec cur_time){
    Animation *cur = NULL;
    // parcours à l'envers pour éviter problème de modification
    for(int i = jeu->animations.nb_elem - 1; i >= 0 ; i--) {
        cur = (Animation *)get_elem(&jeu->animations, i);
        maj_animation(cur);
        if(cur->fin < cur_time.tv_sec*1000 + cur_time.tv_nsec/1.0E6){
            remove_elem(&jeu->animations, i);
        }
    }
    return 0;
}

int maj_jeu(Jeu *jeu, struct timespec cur_time){
    if (jeu->vagues.num_vague > 0 && cur_time.tv_sec >= jeu->prochaine_vague) {
        if(vague_suivante(jeu, cur_time)){
            return 1; // erreur d'allocation
        }
    }
    maj_animations(jeu, cur_time);
    maj_monstres(jeu, cur_time);
    if(maj_projectiles(jeu, cur_time)){
        return 1; // erreur d'allocation
    }
    return 0;
}

int gere_evenements(Jeu *jeu, struct timespec cur_time){
    MLV_Event ev;
    MLV_Button_state state;
    MLV_Input_box *gen_gemme;
    int mouse_x, mouse_y, action;
    int niv;
    char *niv_gemme = NULL;
    while((ev = MLV_get_event(NULL, NULL, NULL, &niv_gemme, &gen_gemme, &mouse_x, &mouse_y, NULL, &state)) != MLV_NONE){
        switch(ev){
            case MLV_MOUSE_BUTTON:
                if(state == MLV_PRESSED){
                    if((action = actions_pressed(jeu, mouse_x, mouse_y, cur_time)))
                        return action; // erreur d'allocation ou action "quitter"
                }else{ // state == MLV_RELEASED
                    if((action = actions_released(jeu, mouse_x, mouse_y, cur_time)))
                        return action; // erreur d'allocation ou action "quitter"
                }
            case MLV_INPUT_BOX:
                if (niv_gemme != NULL && str_to_int(niv_gemme, &niv)) {
                    joueur_creer_gemme(&(jeu->joueur), niv);
                }
            default: break; // nothing
        }
    }
    return 0;
}