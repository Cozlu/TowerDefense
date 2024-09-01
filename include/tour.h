#ifndef TOUR_H
#define TOUR_H

#include "gemme.h"

/**
 * @file tour.h
 * @brief Gère les tours
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    Gemme g;               /* gemme à l'intérieur de la tour*/
    int contient_gemme;    /* 1 si la tour a une gemme 0 sinon*/
    time_ms prochain_proj; /* heure en ms à partir de laquel le prochain projectile peut être lancer*/
} Tour;

/**
 * @brief Intialise une nouvelle tour
 * 
 * @param t contient la tour initialisée
 */
void nouvelle_tour(Tour *t);

/**
 * @brief Ajoute la gemme dans la tour
 * 
 * @param tour tour à modifier
 * @param g gemme à ajouter
 * @return Gemme gemme ajoutée dans la tour
 */
Gemme ajout_gemme_tour(Tour *tour, Gemme g);

/**
 * @brief Retire la gemme de la tour
 * 
 * @param tour tour à modifier
 * @return Gemme gemme ancienement dans la tour
 */
Gemme retire_gemme_tour(Tour *tour);

/**
 * @brief Vérifie si la tour possède une gemme ou non
 *
 * @param tour tour à vérifier
 * @return int 1 si la tour contient une gemme et 0 sinon
 */
int tour_a_gemme(const Tour *tour);

/**
 * @brief Calcule le cout en mana de tour numéro num_tour
 * 
 * @param nb_tour numéro de la tour
 * @return int cout en mana calculé
 */
int cout_tour(int num_tour);

#endif