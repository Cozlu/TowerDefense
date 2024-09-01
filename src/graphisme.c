#include "../include/graphisme.h"
#include <MLV/MLV_all.h>

/**
 * @brief Renvoie la couleur associé à l'élément
 *
 * @param elem élément pour la couleur
 * @return MLV_Color la couleur associée à l'élément
 */
static MLV_Color element_to_color(Element elem){
    switch(elem) {
        case PYRO : return COLOR_PYRO;
        case DENDRO : return COLOR_DENDRO;
        case HYDRO : return COLOR_HYDRO;
        case VAPO : return COLOR_VAPO; // Pyro + Hydro
        case COMBUST : return COLOR_COMBUST; // Pyro + Dendro
        case ENRACIN : return COLOR_ENRACIN; // Hydro + Dendro
        default : return MLV_rgba(0, 0, 0, 0);
    }
}

/**
 * @brief Modifie la propriété alpha de la couleur
 * 
 * @param color couleur à modifier
 * @param new_alpha nouvelle valeur de alpha
 * @return MLV_Color couleur modifée
 */
static MLV_Color change_alpha(MLV_Color color, int new_alpha){
    Uint8 r,g,b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return MLV_convert_rgba_to_color(r, g, b, new_alpha);
}

/**
 * @brief Dessine une case sur la fenêtre
 * 
 * @param li ligne de la case
 * @param col colonne de la case
 * @param couleur couleur de la case
 */
static void dessine_case(int li, int col, MLV_Color couleur) {
    int x = col*TAILLE_CASE;
    int y = li*TAILLE_CASE;
    MLV_draw_filled_rectangle(x, y, TAILLE_CASE, TAILLE_CASE, couleur);
    MLV_draw_rectangle(x, y, TAILLE_CASE, TAILLE_CASE, MLV_COLOR_DARK_GREY);
}

/**
 * @brief Dessine une case d'herbe sur la fenêtre
 * 
 * @param li ligne de la case
 * @param col colonne de la case
 */
static void dessine_case_herbe(int li, int col) {
    MLV_Color couleur;
    if ((li + col) % 2 == 0)
        couleur = MLV_rgba(87, 186, 105, 255);
    else
        couleur = MLV_rgba(79, 168, 95, 255);
    dessine_case(li, col, couleur);
}

/**
 * @brief Convertit la teinte d'une couleur comprise entre 0 et 359 en couleur RGB
 * 
 * @param teinte la teinte à convertir
 * @return la couleur RGB
 */
static MLV_Color teinte_vers_Color(Teinte teinte) {
    int r, g, b;
    teinte_vers_RGB(teinte, &r, &g, &b);
    return MLV_rgba(r, g, b, 255);
}

/**
 * @brief Dessine une gemme sur la fenêtre
 * 
 * @param gemme la gemme à dessiner
 * @param x coordonnée x du coin supérieur gauche de la case de la gemme
 * @param y coordonnée y du coin supérieur gauche de la case de la gemme
 * @param taille taille de la case de la gemme
 */
static void dessine_gemme(Gemme gemme, int x, int y, int taille) {
    char niveau[3];
    MLV_Color couleur = teinte_vers_Color(gemme.teinte);
    if (gemme.pur) {
        MLV_draw_filled_circle(x + taille/2, y + taille/2, taille/3, couleur);
        MLV_draw_circle(x + taille/2, y + taille/2, taille/3, MLV_COLOR_WHITE);
    }
    else {
        MLV_draw_filled_rectangle(x + taille/2 - taille/3, y + taille/2 - taille/3, taille/3 * 2, taille/3 * 2, couleur);
        MLV_draw_rectangle(x + taille/2 - taille/3, y + taille/2 - taille/3, taille/3 * 2, taille/3 * 2, MLV_COLOR_WHITE);
    }
    sprintf(niveau, "%d", gemme.niv);
    MLV_draw_text_box(x, y, taille, taille, niveau, 1, MLV_rgba(0, 0, 0, 0), 
                      MLV_COLOR_DARK_GREY, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

/**
 * @brief Dessine une tour sur la fenêtre
 * 
 * @param tour tour à dessiner
 * @param est_sel 0 si la tour est sélectionnée et 1 sinon
 */
static void dessine_tour(const Tour *tour, int li, int col, int est_sel) {
    int x = col*TAILLE_CASE;
    int y = li*TAILLE_CASE;
    int rayon;
    if (est_sel){
        MLV_draw_filled_circle(x + TAILLE_CASE/2, y + TAILLE_CASE/2, TAILLE_CASE/2 + 2, MLV_COLOR_WHITE);
    }
    MLV_draw_filled_circle(x + TAILLE_CASE/2, y + TAILLE_CASE/2, TAILLE_CASE/2, MLV_rgba(128, 128, 128, 255));
    MLV_draw_circle(x + TAILLE_CASE/2, y + TAILLE_CASE/2, TAILLE_CASE/2, MLV_COLOR_GREY);
    if (tour_a_gemme(tour)) {
        dessine_gemme(tour->g, x, y, TAILLE_CASE);
        if (est_sel && tour->contient_gemme){
            rayon = distance_detection_gemme(tour->g.niv);
            MLV_draw_circle(x + TAILLE_CASE/2, y + TAILLE_CASE/2, rayon*TAILLE_CASE, MLV_COLOR_RED);
        }
    }
}

/**
 * @brief Dessine le terrain du jeu sur la fenêtre
 * 
 * @param terrain terrain à dessiner
 * @param tour_sel coord de la tour sélectionnée, (-1,-1) si aucune ne l'est
 */
static void dessine_terrain(const Terrain *terrain, Coord tour_sel) {
    const Case *c;
    for(int i = 0; i < NB_LIGNES; i++)
        for(int j = 0; j < NB_COLS; j++) {
            c = &terrain->carte[i][j];
            switch (c->type) {
                case VIDE : dessine_case_herbe(i, j); break;
                case CHEMIN: dessine_case(i, j, MLV_rgba(196, 141, 88, 255)); break;
                case NID : dessine_case(i, j, MLV_COLOR_RED); break;
                case CAMP: dessine_case(i, j, MLV_COLOR_ORANGE); break;
                case TOUR : dessine_case_herbe(i, j); break;
            }
        }
    for(int i = 0; i < NB_LIGNES; i++)
        for(int j = 0; j < NB_COLS; j++) {
            c = &terrain->carte[i][j];
            if(c->type == TOUR) {
                int est_sel = 0;
                if(i == tour_sel.li && j == tour_sel.col)
                    est_sel = 1;
                dessine_tour(&c->donnee.tour, i, j, est_sel);
            }
        }

}

/**
 * @brief Dessine l'effet du monstre sur la fenêtre
 * 
 * @param monstre monstre dont l'effet appliqué est à dessiner
 */
static void dessine_effet(const Monstre *monstre) {
    if(monstre->effet.element != NO_EFFECT)
        MLV_draw_filled_circle(monstre->x, monstre->y, 18, element_to_color(monstre->effet.element));
}

/**
 * @brief Dessine le résidu du monstre sur la fenêtre
 * 
 * @param monstre monstre dont l'effet appliqué est à dessiner
 */
static void dessine_residu(const Monstre *monstre){
    MLV_Color couleur;
    if(monstre->effet.residu != NO_EFFECT){
        couleur = element_to_color(monstre->effet.residu);
        MLV_draw_filled_circle(monstre->x + rand()%8 - 4, monstre->y + rand()%8 - 4, 2, couleur);
        MLV_draw_filled_circle(monstre->x + rand()%8 - 4, monstre->y + rand()%8 - 4, 2, couleur);
    }
}


/**
 * @brief Dessine un monstre sur la fenêtre
 * 
 * @param m monstre à dessiner
 */
static void dessine_monstre(const void *m) {
    const Monstre *monstre = (Monstre *) m;
    if(!monstre->est_apparu) return;
    dessine_effet(monstre);
    MLV_draw_filled_circle(monstre->x, monstre->y, 14, MLV_COLOR_DARK_GREY);
    MLV_draw_filled_circle(monstre->x, monstre->y, 10, teinte_vers_Color(monstre->teinte));
    dessine_residu(monstre);
}

/**
 * @brief Dessine la barre de vie d'un monstre sur la fenêtre
 * 
 * @param m monstre dont la barre de vie est à dessiner
 */
static void dessine_barre_vie(const void *m) {
    const Monstre *monstre = (Monstre*) m;
    if(!monstre->est_apparu) return;
    int taille_vie = 36*monstre->pv/monstre->pv_init;
    MLV_draw_filled_rectangle(monstre->x - 20, monstre->y - 30, 40, 10, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(monstre->x - 18, monstre->y - 28, taille_vie, 6, MLV_COLOR_RED);
}

/**
 * @brief Dessine le projectile sur la fenêtre
 * 
 * @param p projectile à dessiner
 */
static void dessine_projectile(const void *p){
    const Projectile *proj = (Projectile *) p;
    MLV_draw_filled_circle(proj->x, proj->y, 8, MLV_COLOR_BLACK);
    MLV_draw_filled_circle(proj->x, proj->y, 6, teinte_vers_Color(proj->gemme.teinte));
}

static void dessine_animation(const void *a){
    const Animation* anim = (Animation *) a;
    MLV_Color color = change_alpha(element_to_color(anim->elem), 150);
    MLV_draw_filled_circle(anim->x, anim->y, anim->rayon*TAILLE_CASE, color);
}

/**
 * @brief Dessine le mana du joueur sur la fenêtre
 * 
 * @param mana mana du joueur avaec sa jauge et son maximum
 */
static void dessine_mana(Mana mana) {
    char qte_max[30];
    char reserve[30];
    char niveau[3];
    int taille_texte, taille_slash, tmp;
    float taille_jauge = mana.reserve*592/mana.qte_max;
    sprintf(qte_max, "%.2f", mana.qte_max);
    sprintf(reserve, "%.2f", mana.reserve);
    sprintf(niveau, "%d", mana.niv);
    MLV_draw_adapted_text_box(30, WIN_HAUTEUR - 100, "Niveau jauge mana : ", 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box("Niveau jauge mana : ", 1, &taille_texte, &tmp);
    MLV_draw_adapted_text_box(30 + taille_texte, WIN_HAUTEUR - 100, niveau, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_draw_filled_rectangle(30, WIN_HAUTEUR - 80, 600, 60, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(34, WIN_HAUTEUR - 76, 592, 52, MLV_rgba(43, 23, 46, 255));
    MLV_draw_filled_rectangle(34, WIN_HAUTEUR - 76, taille_jauge, 52, MLV_rgba(126, 132, 247, 100));
    MLV_draw_adapted_text_box(45, WIN_HAUTEUR - 60, reserve, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box(reserve, 1, &taille_texte, &tmp);
    MLV_draw_adapted_text_box(45 + taille_texte, WIN_HAUTEUR - 60, "/", 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box("/", 1, &taille_slash, &tmp);
    MLV_draw_adapted_text_box(45 + taille_texte + taille_slash, WIN_HAUTEUR - 60, qte_max, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
}

/**
 * @brief Dessine un bouton sur la fenêtre
 * 
 * @param x coordonnée x du coin supérieur gauche du bouton
 * @param y coordonnée y du coin supérieur gauche du bouton
 * @param l longueur du bouton
 * @param h hauteur du bouton
 * @param texte texte du bouton
 * @param c_fond couleur du fond
 * @param couleur couleur du texte et du bord
 */
static void dessine_bouton(int x, int y, int l, int h, char * texte, MLV_Color c_fond, MLV_Color couleur) {
    MLV_draw_text_box(x, y, l, h, texte, 1, couleur, couleur, c_fond, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

/**
 * @brief Dessine le bouton pour quitter sur la fenêtre
 * 
 * @param x coordonnée x du coin supérieur gauche du bouton
 * @param y coordonnée y du coin supérieur gauche du bouton
 * @param l longueur du bouton
 * @param h hauteur du bouton
 */
static void dessine_bouton_quit(int x, int y, int l, int h) {
    MLV_draw_filled_rectangle(x, y, l, h, MLV_rgba(240, 45, 45, 255));
    MLV_draw_filled_rectangle(x + 5, y + 5, l - 10, h - 10, MLV_rgba(240, 100, 100, 255));
    MLV_draw_line(x, y, x + l, y + h, MLV_rgba(240, 45, 45, 255));
    MLV_draw_line(x, y + h, x + l, y, MLV_rgba(240, 45, 45, 255));
}

/**
 * @brief Dessine le menu composé des boutons sur la fenêtre
 * 
 * @param num_vague numéro de la dernière vague lancée
 */
static void dessine_menu(int num_vague) {
    int taille_texte, tmp;
    char vague[3];
    sprintf(vague, "%d", num_vague);
    MLV_draw_filled_rectangle(TAILLE_CASE * NB_COLS, 0, 10, WIN_HAUTEUR, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(0, TAILLE_CASE * NB_LIGNES, TAILLE_CASE * NB_COLS, 10, MLV_COLOR_WHITE);
    dessine_bouton(660, WIN_HAUTEUR - 80, 150, 60, "Agrandir jauge", MLV_rgba(255, 145, 203, 255), MLV_COLOR_WHITE);
    if (num_vague == 0)
        dessine_bouton(820, WIN_HAUTEUR - 80, 150, 60, "Commencer", MLV_rgba(255, 165, 95, 255), MLV_COLOR_WHITE);
    else
        dessine_bouton(820, WIN_HAUTEUR - 80, 250, 60, "Passer à la vague suivante ", MLV_rgba(240, 141, 54, 255), MLV_COLOR_WHITE);
    MLV_draw_adapted_text_box(820, WIN_HAUTEUR - 100, "Vague actuelle : ", 1,  MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box("Vague actuelle : ", 1, &taille_texte, &tmp);
    MLV_draw_adapted_text_box(820 + taille_texte, WIN_HAUTEUR - 100, vague, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    dessine_bouton_quit(WIN_LARGEUR - 50, 0, 50, 50);
    MLV_draw_all_input_boxes();
}

/**
 * @brief Dessine l'inventaire du joueur sur la fenêtre
 * 
 * @param x coordonnée x du coin supérieur gauche de l'inventaire
 * @param y coordonnée y du coin supérieur gauche de l'inventaire
 * @param joueur joueur pour dessiner son inventaire
 * @param gem_sel l'indice de la 1ère gemme sélectionnée, -1 si il n'y en a pas
 */
static void dessine_inv(int x, int y, const Joueur *joueur, int gem_sel) {
    int taille = TAILLE_GEMME_INV;
    int li = 0;
    for(int i = 0; i < GEMME_INV_MAX; i++) {
        if (i % 3 == 0 && i != 0)
            li++;
        if (i == gem_sel)
            MLV_draw_filled_rectangle(x + (i % 3)*taille, y + li*taille, taille, taille, MLV_COLOR_WHITE);
        else
            MLV_draw_filled_rectangle(x + (i % 3)*taille, y + li*taille, taille, taille, MLV_rgba(105, 70, 130, 255));
        MLV_draw_filled_rectangle(x + 2 + (i % 3)*taille, y + 2 + li*taille, taille - 4, taille - 4, MLV_rgba(147, 97, 181, 255));
        if (get_bit(&joueur->contient_gemme, i))
            dessine_gemme(joueur->inventaire[i], x + (i % 3)*taille, y + li*taille, taille);
    }
}

/**
 * @brief Dessine le coût d'une action du joueur sur la fenêtre
 * 
 * @param mouse_x coordonnée x de la souris
 * @param mouse_y coordonnée de la souris
 * @param cout le coût à dessiner
 */
static void dessine_cout(int mouse_x, int mouse_y, int cout){
    int w, h;
    MLV_get_size_of_text("cout= %d", &w, &h, cout);
    MLV_draw_text(mouse_x - w/2, mouse_y + h, "cout= %d", MLV_COLOR_WHITE, cout);
}

/**
 * @brief Dessine la main du joueur lorsqu'elle est vide
 * 
 * @param mouse_x coordonnée x de la souris
 * @param mouse_y coordonnée de la souris
 * @param nb_tour le nombre de tours pour le coût à dessiner
 * @param qte_max la quantité max de mana pour dessiner le coût
 */
static void dessine_main_vide(int mouse_x, int mouse_y, int nb_tours, int qte_max){
    switch(zone_souris(mouse_x, mouse_y)){
        case TERRAIN:
            dessine_cout(mouse_x, mouse_y, cout_tour(nb_tours + 1));
            break;
        case AGGRANDIR_RESERVE:
            dessine_cout(mouse_x, mouse_y, cout_agrandir_mana(qte_max));
            break;
        default:;
    }
}

/**
 * @brief Dessine la main du joueur avec une gemme
 * 
 * @param mouse_x coordonnée x de la souris
 * @param mouse_y coordonnée de la souris
 * @param gem_sel la gemme sélectionnée
 * @param gemme la gemme à dessiner
 * @param joueur l'état courant du joueur
 */
static void dessine_main_gemme(int mouse_x, int mouse_y, int gem_sel, Gemme gemme, const Joueur *joueur){
    float rayon;
    int i_gemme;
    dessine_gemme(gemme, mouse_x - TAILLE_GEMME_INV/2, mouse_y - TAILLE_GEMME_INV/2, TAILLE_GEMME_INV);
    switch(zone_souris(mouse_x, mouse_y)){
        case TERRAIN:
            rayon = distance_detection_gemme(gemme.niv);
            MLV_draw_circle(mouse_x, mouse_y, rayon*TAILLE_CASE, MLV_COLOR_RED);
            break;
        case INVENTAIRE:
            i_gemme = coord_vers_case_inv(mouse_x, mouse_y);
            if(get_bit(&joueur->contient_gemme, i_gemme) && i_gemme != gem_sel)
                dessine_cout(mouse_x, mouse_y, COUT_FUSION_GEMME);
            break;
        default: ;
    }
}


/**
 * @brief Dessine la main du joueur
 * 
 * @param jeu état courant du jeu
 */
static void dessine_main(const Jeu *jeu){
    int mouse_x, mouse_y;
    MLV_get_mouse_position(&mouse_x, &mouse_y);
    if(jeu->gem_sel != -1 && get_bit(&jeu->joueur.contient_gemme, jeu->gem_sel)){
        dessine_main_gemme(mouse_x, mouse_y, jeu->gem_sel, jeu->joueur.inventaire[jeu->gem_sel], &jeu->joueur);
    }else{
        dessine_main_vide(mouse_x, mouse_y, jeu->terrain.nb_tours, jeu->joueur.jauge.qte_max);
    }
}

void dessine_jeu(const Jeu *jeu) {
    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(0, 0, WIN_LARGEUR, WIN_HAUTEUR, BACKGROUND_COLOR);
    dessine_menu(jeu->vagues.num_vague);
    dessine_mana(jeu->joueur.jauge);
    dessine_terrain(&jeu->terrain, jeu->tour_sel);
    for_each_elem(&jeu->vagues.vague, &dessine_monstre);
    for_each_elem(&jeu->vagues.vague, &dessine_barre_vie);
    for_each_elem(&jeu->projectiles.proj, &dessine_projectile);
    for_each_elem(&jeu->animations, &dessine_animation);
    dessine_inv(WIN_LARGEUR - 255, 250, &jeu->joueur, jeu->gem_sel);
    dessine_main(jeu);
    MLV_update_window();
}

void dessine_ecran_fin(int nb_degats, int nb_vagues) {
    int taille_texte, h;
    char nb_degats_c[30];
    char nb_vagues_c[30];
    sprintf(nb_degats_c, "%d", nb_degats);
    sprintf(nb_vagues_c, "%d", nb_vagues);
    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(0, 0, WIN_LARGEUR, WIN_HAUTEUR, BACKGROUND_COLOR);
    MLV_get_size_of_adapted_text_box("Fin de la partie ! Voici le score : ", 1, &taille_texte, &h);
    MLV_draw_adapted_text_box(WIN_LARGEUR/2 - taille_texte/2, WIN_HAUTEUR/2, "Fin de la partie ! Voici le score : ", 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box("Dégâts totaux infligés : ", 1, &taille_texte, &h);
    MLV_draw_adapted_text_box(WIN_LARGEUR/2 - taille_texte/2, WIN_HAUTEUR/2 + 15, "Dégâts totaux infligés : ", 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_draw_adapted_text_box(WIN_LARGEUR/2 + taille_texte/2, WIN_HAUTEUR/2 + 15, nb_degats_c, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_get_size_of_adapted_text_box("Nombre de vagues endurées : ", 1, &taille_texte, &h);
    MLV_draw_adapted_text_box(WIN_LARGEUR/2 - taille_texte/2, WIN_HAUTEUR/2 + 30, "Nombre de vagues endurées : ", 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_draw_adapted_text_box(WIN_LARGEUR/2 + taille_texte/2, WIN_HAUTEUR/2 + 30, nb_vagues_c, 1, MLV_rgba(0, 0, 0, 0), 
                              MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER);
    MLV_update_window();
}
