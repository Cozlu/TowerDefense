#ifndef GEMME_H
#define GEMME_H

#include "element.h"

/**
 * @file gemme.h
 * @brief Gère les gemmes du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    Teinte teinte;           /* couleur entre 0 et 360 */
    unsigned char niv;       /* niveau positif*/
    unsigned char pur;       /* 1 -> pur  0 -> mixte*/
} Gemme; 

#define GEMME_FAUSSE init_gemme(-1, 0, 0);

/**
 * @brief Teste si deux gemmes sont égales
 * 
 * @param gem1 1ère gemme à comparer
 * @param gem2 2ème gemme à comparer
 * @return int 1 si les deux gemmes sont égales, 0 sinon
 */
int gemme_egales(const Gemme *gem1, const Gemme *gem2);

/**
 * @brief Intialise une nouvelle gemme
 * 
 * @param teinte couleur: entier entre 0 et 359 
 * @param niv niveau de la gemme (positif)
 * @param pur 1 si pur 0 sinon
 * @return Gemme nouvelle gemme initialisée
 */
Gemme init_gemme(Teinte teinte, unsigned char niv, unsigned char pur);

/**
 * @brief Créée une nouvelle gemme aléatoire à partir d'un niveau 
 * 
 * @details une nouvelle gemme est forcément pure
 * @param niv niveau de la gemme (positif)
 * @return Gemme nouvelle gemme
 */
Gemme random_gemme(int niv);

/**
 * @brief Calcule le prix en mana d'une nouvelle gemme de niveau niv
 *
 * @param niv niveau de la gemme pour calculer le prix
 * @return int le prix de la gemme en mana
 */
int prix_gemme(unsigned int niv);


/**
 * @brief Renvoie l'élément de la gemme
 *
 * @param g la gemme à tester
 * @return Element l'élément correspondant à la gemme
 */
Element element_gemme(const Gemme *g);

/**
 * @brief Calcule le délai en ms entre 2 tir d'une gemme de niveau niv
 * 
 * @param niv niveau de la gemme pour calculer le délai
 * @return time_ms le délai calculé
 */
time_ms delai_tir_gemme(unsigned int niv);

/**
 * @brief Calcule la distance en nb_cases pour les tirs d'une gemme de niveau niv
 *
 * @param niv niveau de la gemme pour calculer la distance
 * @return float la distance calculée
 */
float distance_detection_gemme(unsigned int niv);

/**
 * @brief Teste si deux gemmes peuvent fusionner
 * @param gem1 1ère gemme à comparer
 * @param gem2 2ème gemme à comparer
 * @return int 1 si les deux gemmes sont fusionnables, 0 sinon
 */
int peuvent_fusionner(const Gemme *gem1, const Gemme *gem2);

/**
 * @brief Fusionne les deux gemmes spécifiées
 * 
 * @warning on suppose que les deux gemmes peuvent effectivement fusionner
 * @param gem1 1ère gemme à fusionner
 * @param gem2 2ème gemme à fusionner
 * @return Gemme la gemme résultant de la fusion
 */
Gemme fusion_gemmes(const Gemme *gem1, const Gemme *gem2);

#endif
