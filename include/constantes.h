#ifndef CONSTANTES_H
#define CONSTANTES_H

/**
 * @file constantes.h
 * @brief Définition de toutes les constantes du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

#define _POSIX_C_SOURCE 199309L

// constantes de dimensions
#define WIN_LARGEUR 1400        /* largeur de la fenetre*/
#define WIN_HAUTEUR 1000        /* hauteur de la fenetre*/
#define NB_LIGNES 22            /* nombre de lignes du terrain */
#define NB_COLS 28              /* nombre de colonnes du terrain*/
#define TAILLE_CASE 40            /* taille des cases du terrain*/

// constante de tailles 
#define MAX_TOURS (NB_LIGNES*NB_COLS)  /* nombre maximum de tour possible sur le terrain*/
#define GEMME_INV_MAX 27         /* nombre de gemme maximal dans l'inventaire*/


// constante de graphisme
#define BACKGROUND_COLOR MLV_rgba(43, 23, 46, 255)
#define TAILLE_GEMME_INV (TAILLE_CASE*2)
#define TEMPS_ANIMATION 500      /* temps en ms durant lequel on affiche les animation */

#define COLOR_PYRO MLV_COLOR_ORANGE
#define COLOR_DENDRO MLV_COLOR_GREEN
#define COLOR_HYDRO MLV_COLOR_BLUE
#define COLOR_VAPO MLV_COLOR_AQUAMARINE
#define COLOR_ENRACIN MLV_COLOR_DARK_GREEN
#define COLOR_COMBUST MLV_COLOR_RED

// constante d'équilibrage
#define CONSTANTE_DEGATS 75            /* constante multiplicative des dégâts des projectiles*/
#define CONSTANTE_PV 100                /* constante multiplicative des pv des monstres*/
#define COUT_FUSION_GEMME 100         /* prix en mana de la fusion de 2 gemmes*/
#define TEMPS_ENTRE_VAGUE 35  /* 35 secondes entre chaque vagues*/
#define CONSTANTE_DELAI_TIR 1  /* constante multiplicative du delai entre chaque tir*/
#define DELAI_UTIL_GEMME 2           /* temps en s avant lequel on peut reposer une gemme après l'avoir retirer*/

// constante diverse
#define FPS 60                   /* nombre de rafraichissement pas secondes*/
#define DISTANCE_PAR_MAJ (TAILLE_CASE /(double) FPS)  /* == 1 cases par secondes*/
#define PI 3.14159265358979
#define MIN(a,b) ((a) < (b)? (a) : (b))

#endif