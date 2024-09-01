#ifndef TERRAIN_H
#define TERRAIN_H

#include "tour.h"
#include "constantes.h"

/**
 * @file terrain.h
 * @brief Gère le terrain (sans les monstres) 
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/**
 * représente le contenu d'une case \n
 * Pour les chemin, la direction est celle vers la prochaine case du chemin
 * les chemins doivents être dans le même ordre que Direction, et adjacents
 */
typedef enum {
    VIDE,
    CHEMIN,
    NID,
    CAMP,
    TOUR,
} TypeCase;

typedef enum {
    HAUT,
    BAS,
    GAUCHE,
    DROITE
} Direction;

typedef struct {
    int li, col;
} Coord;

typedef struct{
    Direction suiv, prec;
} Chemin;

typedef struct {
    TypeCase type;
    union{
        Chemin chemin;
        Tour tour;
    } donnee;
} Case;

typedef struct {
    int taille_chemin;                  /* nombre de case dans le chemin */
    int nb_tours;                       /* nombre de tour dans le tableau */
    Case carte[NB_LIGNES][NB_COLS];     /* tableau des cases du terrain */
    Coord nid;
    Coord camp;
} Terrain;

/**
 * @brief Renvoie la composante colonne correspondant à la direction
 *
 * @return int la composante colonne correspondante
 */
int dir_col(Direction dir);

/**
 * @brief Renvoie la composante ligne correspondant à la direction
 *
 * @return int la composante ligne correspondante
 */
int dir_li(Direction dir);

/**
 * @brief Calcule la direction opposée à la direction spécifiée
 *
 * @return Direction la direction opposée
 */
Direction inverse_dir(Direction dir);

/**
 * @brief Donne la direction de la case du chemin
 * 
 * @param terrain terrain du jeu
 * @param c coordonnées de la case
 * @return Direction direction de la case
 */
Direction chemin_vers_dir(const Terrain *terrain, Coord c);

/**
 * @brief Construit une case à partir des directions des cases suivantes et précédentes
 * 
 * @param suiv direction de la case suivante
 * @param prec direction de la case précédente
 * @return Case la case construite
 */
Case dir_vers_chemin(Direction suiv, Direction prec);

/**
 * @brief Teste si une case est sur le terrain
 * 
 * @param c coordonnées de la case à tester
 * @return int 1 si la case est sur le terrain 0 sinon
 */
int case_valide(const Coord *c);

/**
 * @brief Teste si deux cases sont égales
 * 
 * @param c1 coordonnées de la 1ère case à tester
 * @param c2 coordonnées de la 2ème case à tester
 * @return int 1 si les cases sont égales, 0 sinon
 */
int case_egal(const Coord *c1, const Coord *c2);

/**
 * @brief Construit aléatoirement la carte du jeu
 * 
 * @param terrain terrain dont la carte est à construire
 */
void construit_carte(Terrain * terrain);

/**
 * @brief Initialise le terrain du jeu
 * 
 * @return Terrain terrain initialisé
 */
Terrain init_terrain();


/**
 * @brief Calcule la distance de manhattan entre deux cases
 * 
 * @param i1 ligne de la 1ère case
 * @param j1 colonne de la 1ère case
 * @param i2 ligne de la 2ème case
 * @param j2 colonne de la 2ème case
 * @return int distance de manhattan entre deux cases
 */
int dist_man(int i1, int j1, int i2, int j2);

/**
 * @brief Calcule la distance euclidienne en nombred de pixel entre 2 points (x1, y1) et (x2, y2)
 * 
 * @param x1 x du premier point
 * @param y1 y du premier point
 * @param x2 x du deuxième point
 * @param y2 y du deuxième point
 * @return float distance en pixel
 */
float dist_eucl(float x1, float y1, float x2, float y2) ;

/**
 * @brief Calcule la distance euclidienne en nombre de cases entre 2 points (x1, y1) et (x2, y2)
 * 
 * @param x1 x du premier point
 * @param y1 y du premier point
 * @param x2 x du deuxième point
 * @param y2 y du deuxième point
 * @return float distance en nombre de cases
 */
float dist_eucl_case(float x1, float y1, float x2, float y2);

/**
 * @brief Converti les coordonnées réelles vers des coordonnées de case
 * 
 * @param x coordonnée réelle x à convertir
 * @param y coordonnée réelle y à convertir
 * @param result pour stocker les coordonnées converties
 * @return int 1 si les pointeurs sont null et 0 sinon
 */
int coord_vers_case(float x, float y, Coord *result);

/**
 * @brief Converti les indices de case en coordonnées réelles du centre de la case
 * 
 * @param li indice de la ligne à convertir
 * @param col indice de la colonne à convertir
 * @param x pointeur vers la coordonnée réelle x du centre
 * @param y pointeur vers la coordonnée réelle y du centre
 * @return int 1 si les pointeurs sont null et 0 sinon
 */
int case_vers_coord_centre(int li, int col, float *x, float *y);

/**
 * @brief Vérifie si la case est libre
 * 
 * @param terrain Terrain 
 * @param c coordonnées de la case à vérifier
 * @return 1 si la case est libre et 0 sinon
 */
int case_est_libre(const Terrain *terrain, const Coord *c);

/**
 * @brief Vérifie si une tour se trouve sur la case spécifiée
 * 
 * @param terrain le terrain du jeu
 * @param li la ligne de la case
 * @param col la colonne de la case
 * @return la tour qui se trouve sur la case, NULL si aucune tour n'est à cette case
 */
Tour *tour_case(Terrain *terrain, int li, int col);

#endif
