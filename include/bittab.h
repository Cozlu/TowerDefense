#ifndef BITTAB_H
#define BITTAB_H

/**
 * @file bittab.h
 * @brief Gère les tableaux de bit
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/**
 * @brief Structure qui représente un tableau de bit de taille __INT_MAX__
 * en utilisant les opérations bit à bit
 */
typedef unsigned int Bittab;

/**
 * @brief Renvoie un nouveau tableau vide (que des 0)
 * 
 * @return Bittab nouveau tableau vide
 */
Bittab init_Bittab();

/**
 * @brief Renvoie le ième bit du tableau c'est à dire tab[i]
 * 
 * @param tab Tableau de bit
 * @param i indice dans le tableau
 * @return char tab[i] = 1 ou 0, 
 */
char get_bit(const Bittab *tab, unsigned int i);

/**
 * @brief Affecte à tab[i] la valeur désirée (0 ou 1)
 * 
 * @param tab Tableau de bit
 * @param i indice dans le tableau
 * @param value 1 ou 0
 */
void set_bit(Bittab *tab, unsigned int i, char value);

#endif