#include "../include/terrain.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int dist_man(int i1, int j1, int i2, int j2) {
    return abs(i1 - i2) + abs(j1 - j2);
}

float dist_eucl(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

float dist_eucl_case(float x1, float y1, float x2, float y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))/TAILLE_CASE;
}

int coord_vers_case(float x, float y, Coord *result) {
    if (!result)
        return 1;
    result->col = x/TAILLE_CASE;
    result->li = y/TAILLE_CASE;
    return 0;
}

int case_vers_coord_centre(int li, int col, float *x, float *y) {
    if (!x || !y)
        return 1;
    *x = col*TAILLE_CASE + TAILLE_CASE/2;
    *y = li*TAILLE_CASE + TAILLE_CASE/2;
    return 0;
}

int case_egal(const Coord *c1, const Coord *c2){
    return c1->col == c2->col && c1->li == c2->li;
}

int case_valide(const Coord *c){
    return 0 <= c->col && c->col < NB_COLS &&
           0 <= c->li && c->li < NB_LIGNES;
}

int dir_col(Direction dir){
    /* ici on créée une variable static car on garde les mêmes valeurs
    durant la construction de la carte */
    static const int tab_dir_col[] = {0, 0, -1, 1};
    return tab_dir_col[dir];
}

int dir_li(Direction dir){
    static const int tab_dir_li[] = {-1, 1, 0, 0};
    return tab_dir_li[dir];
}

Direction inverse_dir(Direction dir){
    switch (dir){
        case HAUT: return BAS;
        case DROITE: return GAUCHE;
        case BAS: return HAUT;
        case GAUCHE: return DROITE;
        default:
            fprintf(stderr, "%d n'est pas une direction connue\n", dir);
            return -1;
    }
}

Direction chemin_vers_dir(const Terrain *terrain, Coord c) {
    Case cur = terrain->carte[c.li][c.col];
    if(cur.type != CHEMIN && cur.type != NID && cur.type != CAMP){
        fprintf(stderr, "%d,%d n'est pas un chemin\n", c.col, c.li);
    }
    return cur.donnee.chemin.suiv;
}

Case dir_vers_chemin(Direction suiv, Direction prec){
    Case c;
    c.type = CHEMIN;
    c.donnee.chemin.suiv = suiv;
    c.donnee.chemin.prec = prec;
    return c;
}

/**
 * @brief Vérifie si la case est dans la zone de création du chemin
 *
 * @param c coordonnées de la case à vérifier 
 * @return int 1 si la case est une case de chemin valide, 0 sinon
 */
static int case_valide_chemin(const Coord *c){
    return 2 <= c->col && c->col < NB_COLS - 2 &&
           2 <= c->li && c->li < NB_LIGNES - 2;
}

/**
 * @brief Donne la case qui précède la case courante du chemin
 *
 * @param terrain état courant du terrain
 * @param courante coordonnées de la case courante
 * @param precedente utilisé pour stocker la case précédente
 * @return int 1 si la case est une case de chemin valide, 0 sinon
 */
static int precedente_case(const Terrain *terrain, const Coord *courante, Coord *precedente){
    Case c = terrain->carte[courante->li][courante->col];
    Direction dir;
    if(c.type != CHEMIN && c.type != NID && c.type != CAMP){
        fprintf(stderr, "case (%d,%d) pas un chemin type =%d\n", courante->li, courante->col, c.type);
        return 1;
    }
    dir = c.donnee.chemin.prec;
    precedente->li = courante->li + dir_li(dir);
    precedente->col = courante->col + dir_col(dir);
    return 0;
}

/**
 * @brief Vérifie si la case est une des deux cases du chemin qui précède la case courante
 * 
 * @param terrain terrain du jeu
 * @param test coordonnées de la case à vérifier
 * @return 0 si la case n'est pas une case qui précède la case courante et 1 sinon
 */
static int est_case_prec(const Terrain *terrain, const Coord *test) {
    Coord prec, prec2;
    if(case_egal(&terrain->camp, test)){
        return 1;
    }
    precedente_case(terrain, &terrain->camp, &prec);
    if (case_egal(test, &prec)){
        return 1;
    }
    if(terrain->taille_chemin >= 2){
        precedente_case(terrain, &prec, &prec2);  
        return case_egal(test, &prec2);
    }
    return 0;
}

/**
 * @brief Vérifie si les voisins de la case courante à distance de manhattan inférieur ou égale à 2 sont des cases chemin
 * 
 * @param terrain terrain du jeu
 * @param test case que l'on vérifie
 * @return 1 si une case est à distance de manhattan inférieur ou égale à 2 par rapport à la case courante 0 sinon
 */
static int case_proche(const Terrain *terrain, const Coord *test) {
    Coord tmp;
    int dist;
    /* on teste les voisins dans un carré de taille 5
    avec pour centre la case courante du chemin */
    for(int i = -2; i < 3; i++)
        for(int j = -2; j < 3; j++){
            tmp.li = test->li + i;
            tmp.col = test->col + j;
            dist = dist_man(test->li, test->col, tmp.li, tmp.col);
            /* si la distance est inférieure ou égale à 2, que la case n'est pas vide
            et que la case n'est pas une précédente alors il faut arrêter de construire
            dans cette direction */
            if (dist <= 2 && terrain->carte[tmp.li][tmp.col].type != VIDE && 
                !est_case_prec(terrain, &tmp)){
                return 1;
            }
        }
    return 0;
}

/**
 * @brief Calcule l'étendue dans une direction
 * 
 * @param terrain terrain du jeu
 * @param taille taille maximale de la carte dans une direction
 * @param dir direction courante
 * @return int étendue calculée
 */
static int calc_etendu(const Terrain *terrain, int taille, Direction dir) {
    int etendue = 0;
    Coord tmp = terrain->camp;
    for(int i = 0; i < taille - 2; i++) {
        tmp.li += dir_li(dir);
        tmp.col += dir_col(dir);
        if (!case_valide_chemin(&tmp) || case_proche(terrain, &tmp)) {
            /* on arrête le calcul si l'étendue sort des limites 
            ou si une case du chemin est trop proche */
            return etendue;
        }
        etendue++;
    }
    return etendue;
}

/**
 * @brief Ajoute n cases à la fin du tableau route
 * 
 * @param terrain terrain du jeu
 * @param n nombre de cases à ajouter
 * @param dir direction courante
 */
static void ajout_cases(Terrain * terrain, int n, Direction dir) {
    Coord *courante = &terrain->camp;
    terrain->carte[courante->li][courante->col].donnee.chemin.suiv = dir;
    Direction inverse = inverse_dir(dir);
    for(int i = 0; i < n; i++) {
        courante->li += dir_li(dir);
        courante->col += dir_col(dir);
        terrain->carte[courante->li][courante->col] = dir_vers_chemin(dir, inverse);
        terrain->taille_chemin++;
    }
}

/**
 * @brief Tire aléatoirement une des étendues du tableau
 * 
 * @param etendues tableau des étendues calculées
 * @param min_i limite minimale du tableau
 * @param max_i limite maximale du tableau
 * @return int indice dans le tableau des étendues de l'étendue tiré au hasard
 */
static int tirage_dir(int * etendues, int min_i, int max_i) {
    int tot = 0;
    int res = 0;
    int min = 0; // somme des étendues précédent la courante
    int proba;
    for(int i = min_i; i < max_i; i++)
        tot += etendues[i]; // somme des étendues
    if (tot == 0)
        return min_i;
    proba = rand() % tot; // on tire un nombre entre 0 et la somme des étendues
    for(int i = min_i; i < max_i; i++) {
        /* la probabilité de tirage d'une étendude étant proportionnelle 
        à elle même, une étendue est tirée si le nombre est entre la somme des
        précédentes étendues dans le tableau et l'étendue courante 
        + la somme des étendues précédentes  */
        if (proba >= min && proba < (etendues[i] + min)) {
            /* ex : on a 4 étendues : 1, 2, 3, 4 
            on va tirer l'étendue de taille 3 si le nombre tiré
            est entre 1 + 2 = 3 = min et 3 + min = 6*/
            res = i;
            break;
        }
        min += etendues[i];
    }
    return res;
}

/**
 * @brief Calcule la somme des n 1 ou 0 tiré aléatoirement
 * 
 * @param n nombre de valeurs à tirer
 * @return int somme des 1 et 0 tirés
 */
static int tirage_nb(int n) {
    int res = 0;
    int proba;
    for(int i = 0; i < n; i++) {
        proba = rand() % 4;
        res += (proba <= 2) ? 1 : 0;
    }
    return res;
}

/**
 * @brief Compare le maximum entre a et b
 * 
 * @param a 1er nombre à comparer
 * @param b 2ème nombre à comparer
 * @return int valeur maximale entre a et b, si a et b sont égaux alors b
 */
static int max_val(int a, int b) {
    if (a > b)
        return a;
    return b;
}

/**
 * @brief Vide la carte du terrain
 * 
 * @param terrain terrain du jeu
 */
static void vide_carte(Terrain * terrain) {
    for (int i = 0; i < NB_LIGNES; i++)
        for (int j = 0; j < NB_COLS; j++)
            terrain->carte[i][j].type = VIDE;
    terrain->taille_chemin = 0;
}

/**
 * @brief Vérifie si le chemin est valide
 *
 * @param nb_virages le nombre de virages dans le chemin
 * @param taille_chemin la taille finale du chemin
 * @return int 1 si le chemin est valide 0 sinon
 */
static int chemin_valide(int nb_virages, int taille_chemin){
    return nb_virages >= 7 && taille_chemin >= 75;
}

void construit_carte(Terrain * terrain) {
    Direction dirs[4] = {HAUT, BAS, GAUCHE, DROITE};
    Direction dir;
    int etendues[4];
    int tirage, nb_virages;
    Coord *courante = &terrain->camp;
    while (!chemin_valide(nb_virages, terrain->taille_chemin)) {
        nb_virages = 0;
        vide_carte(terrain); // on vide le terrain
        /* tirage de la case du nid qui sera le point
        de départ du chemin */
        courante->li = rand() % (NB_LIGNES - 4) + 2;
        courante->col = rand() % (NB_COLS - 4) + 2;
        
        terrain->nid = *courante;
        terrain->taille_chemin = 1;
        /* calcul des étendues dans les 4 directions à
        partir du nid tiré précédement */
        etendues[HAUT] = calc_etendu(terrain, NB_COLS, HAUT);
        etendues[BAS] = calc_etendu(terrain, NB_COLS, BAS);
        etendues[GAUCHE] = calc_etendu(terrain, NB_LIGNES, GAUCHE);
        etendues[DROITE] = calc_etendu(terrain, NB_LIGNES, DROITE);

        tirage = tirage_dir(etendues, 0, 4); // tirage d'une des 4 étendues
        terrain->carte[courante->li][courante->col].type = NID; // ajout du nid à la carte
        terrain->carte[courante->li][courante->col].donnee.chemin.suiv = dirs[tirage];
        while (1) { 
            dir = dirs[tirage];
            if (etendues[tirage] <= 2)
                break; // on arrête l'algo si l'étendue est trop petite
            ajout_cases(terrain, max_val(tirage_nb(etendues[tirage]), 3), dir);
            if (chemin_valide(nb_virages, terrain->taille_chemin))
                break; // si le chemin final n'est pas valide on arrête et recommence l'algo
            /* on réutilise une partie du tableau etendues
            pour deux directions seulement */
            if (dir == HAUT || dir == BAS) {
                etendues[GAUCHE] = calc_etendu(terrain, NB_COLS, GAUCHE);
                etendues[DROITE] = calc_etendu(terrain, NB_COLS, DROITE);
                tirage = tirage_dir(etendues, 2, 4);
            } 
            if (dir == GAUCHE || dir == DROITE) {
                etendues[HAUT] = calc_etendu(terrain, NB_LIGNES, HAUT);
                etendues[BAS] = calc_etendu(terrain, NB_LIGNES, BAS);
                tirage = tirage_dir(etendues, 0, 2);
            }
            nb_virages++;
        }
    }
    terrain->carte[courante->li][courante->col].type = CAMP;
}

Terrain init_terrain() {
    Terrain terrain;
    terrain.taille_chemin = 0;
    terrain.nb_tours = 0;
    construit_carte(&terrain);
    return terrain;
}

int case_est_libre(const Terrain *terrain, const Coord *c){
    return terrain->carte[c->li][c->col].type == VIDE;
}

Tour *tour_case(Terrain *terrain, int li, int col){
    Case *c = &terrain->carte[li][col];
    if(c->type != TOUR)
        return NULL;
    return &c->donnee.tour;
}
