#ifndef JEU_H
#define JEU_H

#include <time.h>

#include "terrain.h"
#include "projectile.h"
#include "monstre.h"
#include "joueur.h"

/**
 * @file jeu.h
 * @brief Fonctions haut niveau qui gèrent l'entièrté du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    int gem_sel;
    Coord tour_sel;
    time_t prochaine_vague;  /* temps auquel il faudra lancer la prochaine vague */
    Joueur joueur;
    Terrain terrain;
    TabVagues vagues;
    TabProj projectiles;
    TabAnimation animations;
} Jeu;

/**
 * @brief Initialise le jeu
 * 
 * @param jeu le jeu contenant le terrain, joueur, projectiles, vagues etc
 * @return 1 si il y a eu un problème d'allocation et 0 sinon
 */
int init_jeu(Jeu * jeu);

/**
 * @brief Libère la mémoire allouée aux structures du jeu
 * 
 * @param jeu le jeu contenant le terrain, joueur, projectiles, vagues etc 
 */
void libere_jeu(Jeu *jeu);

/**
 * @brief Met à jour toutes les structures du jeu
 * @details cette fonction est à appeler à chaque rafraichissement
 * @param jeu état courant du jeu
 * @param cur_time heure actuelle
 * @return int 1 si erreur d'allocation \n
 * 0 sinon
 */
int maj_jeu(Jeu *jeu, struct timespec cur_time);

/**
 * @brief Lance la vague de monstre suivante 
 * @param jeu état courant du jeu
 * @param cur_time heure actuelle
 * @return int 1 si erreur d'allocation \n
 * 0 sinon
 */
int vague_suivante(Jeu *jeu, struct timespec cur_time);

/**
 * @brief Gère les événements souris de la file d'évenement et met à jour le jeu en conséquence
 * 
 * @param jeu état courant du jeu
 * @param cur_time heure actuelle
 * @return int 1 s'il y a une erreur d'allocation, -1 pour quitter le jeu et 0 sinon
 */
int gere_evenements(Jeu *jeu,  struct timespec cur_time);

#endif