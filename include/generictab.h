#ifndef GENERICTAB_H
#define GENERICTAB_H

#include <stdlib.h>

/**
 * @file generictab.h
 * @brief Gère les tableaux génériques à taille variable d'éléments quelconques
 * @details les éléments ne sont pas triés, 
 * l'ordre n'est pas conservé 
 * et les éléments gardés au début de la liste
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/**
 * @brief tableau aloué sur le tas avec ordre non conservé
 */
typedef struct {
    void *elems;      /* tableau des éléments */
    size_t elem_size; /* tailles des éléments */
    int nb_elem;      /* nombre d'élément actuellement dans le tableau */
    int nb_max;       /* taille max du tableau (avant réalocation) */
    char *name;       /* nom du tableau (pour debug) */
} GenericTab;

/**
 * @brief Initialise un nouveau tableau avec nb_elem de taille elem_size
 * 
 * @param tab tableau à initialiser
 * @param elem_size taille des élements
 * @param nb_max nombre d'éléments max
 * @return int 1 en cas d'erreur d'allocation, 0 sinon
 */
int init_GenericTab(GenericTab *tab, size_t elem_size, int nb_max);

/**
 * @brief Teste si l'indice est dans le tableau
 *
 * @param tab tableau sur lequel tester l'indice
 * @param i l'indice à tester
 * @return int 1 si l'indice est dans le tableau 0 sinon 
 */
int correct_index(const GenericTab *tab, int i);

/**
 * @brief Donne l'élément du tableau à l'indice i (équiv à tab[i])
 * 
 * @param tab tableau pour obtenir l'élément
 * @param i indice de l'élément
 * @return void* pointeur sur l'élément, NULL si l'indice n'est pas dans le tableau
 */
void *get_elem(const GenericTab *tab, int i);

/**
 * @brief Donne le dernier élément du tableau
 * 
 * @param tab tableau pour obtenir l'élément
 * @return void* pointeur sur l'élément
 */
void *get_last_elem(const GenericTab *tab);

/**
 * @brief Donne le premier element du tableau
 * 
 * @param tab tableau pour obtenir l'élément
 * @return void* pointeur sur l'élément
 */
void *get_first_elem(const GenericTab *tab);

/**
 * @brief Ajoute un élément à la fin du tableau(effectue une recopie)
 * 
 * @param tab tableau où l'on ajoute l'élément
 * @param elem élément à ajouter
 * @return int 1 en cas d'erreur d'allocation, 0 sinon
 */
int add_elem(GenericTab *tab, void *elem);

/**
 * @brief Affecte à la case d'indice i du tableau un élément (effectue une recopie)
 * 
 * @param tab tableau où l'on affetce l'élément
 * @param i indice de la case
 * @param elem element à affecter
 * @return 1 en cas d'erreur d'allocation, 0 sinon
 */
int set_elem(GenericTab *tab, int i, void *elem);

/**
 * @brief Supprime l'élément tab[i] et le remplace par tab[nb_elem - 1]
 * 
 * @param tab tableau à modifier
 * @param i indice de l'élément à supprimer
 * @return void* le pointeur sur le nouveau tab[i]
 */
void *remove_elem(GenericTab *tab, int i);

/**
 * @brief Echange les éléments en tab[i] et tab[j]
 * 
 * @param tab tableau à modifier
 * @param i indice du 1er élément à échanger
 * @param j indice du 2ème élément à échanger
 * @return int -1 si les indices sont faux, 1 si erreur d'allocation, 0 sinon
 */
int echange_elem(GenericTab *tab, int i, int j);

/**
 * @brief Appelle la fonction f sur chaque élément de tab
 * 
 * @param tab tableau
 * @param f fonction à appeler sur chaque élément
 */
void for_each_elem(const GenericTab *tab, void (*f)(const void*));

/**
 * @brief Filtre les élements du tableau
 * 
 * @param tab tableau à filtrer
 * @param filter fonction de filtrage (1 si il faut retirer l'élément, 0 sinon)
 * @return int nombre d'éléments retirés
 */
int filter_elem(GenericTab *tab, int (*filter)(const void*));

/**
 * @brief Libère la mémoire allouée au tableau
 *
 * @param tab tableau à libérer
 */
void free_GenericTab(GenericTab *tab);

#endif
