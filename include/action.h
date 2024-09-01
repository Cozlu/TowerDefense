#ifndef ACTION_H
#define ACTION_H

#include "constantes.h"
#include "terrain.h"
#include "joueur.h"
#include "monstre.h"
#include "jeu.h"

#include <time.h>

/**
 * @file action.h
 * @brief Permet de gérer les interactions joueur/jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

// représente toutes les zones de l'écran
typedef enum {
    TERRAIN = 1,
    INVENTAIRE,
    AGGRANDIR_RESERVE,
    VAGUE_SUIVANTE,
    CROIX
} Zones_Clic; 

/**
 * @brief Renvoie l'indice de la case cliquée
 *
 * @param mouse_x coordonnée x de la souris
 * @param mouse_y coordonnée y de la souris
 * @return int l'indice de la case en fonction de l'emplacement (x, y) de la souris
 */
int coord_vers_case_inv(int mouse_x, int mouse_y);

/**
 * @brief Renvoie une zone selon les coordonnées du clic
 * 
 * @param mouse_x coordonnée x du clic de la souris
 * @param mouse_y coordonnée y du clic de la souris
 * @return Zones_Clic la zone cliquée par le joueur
 */
Zones_Clic zone_souris(int mouse_x, int mouse_y);

/**
 * @brief Effectue toutes les actions d'un clic pressé du joueur durant la partie
 * 
 * @param jeu le jeu à modifier selon l'action
 * @param mouse_x coordonnée x du clic de la souris
 * @param mouse_y coordonnée y du clic de la souris
 * @param cur_time heure actuel
 * @return int 1 si il y a eu une erreur d'allocation, -1 pour quitter le jeu, 0 sinon
 */
int actions_pressed(Jeu *jeu, int mouse_x, int mouse_y, struct timespec cur_time);

/**
 * @brief Effectue toutes les actions d'un clic relaché du joueur durant la partie
 * 
 * @param jeu le jeu à modifier selon l'action
 * @param mouse_x coordonnée x du clic de la souris
 * @param mouse_y coordonnée y du clic de la souris
 * @param cur_time heure actuel
 * @return int 1 si il y a eu une erreur d'allocation, -1 pour quitter le jeu, 0 sinon
 */
int actions_released(Jeu *jeu, int mouse_x, int mouse_y, struct timespec cur_time);

#endif