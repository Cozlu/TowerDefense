#ifndef JOUEUR_H
#define JOUEUR_H

#include "gemme.h"
#include "terrain.h"
#include "constantes.h"
#include "bittab.h"

/**
 * @file joueur.h
 * @brief Gère l'inventaire et toutes les actions du joueur
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    int niv;         /* niveau de la réserve*/
    float qte_max;   // == 2000 * 1.4^niv
    float reserve;   /* qte de mana en réserve*/
    double cout; // garde en mémoire un calcul coûteux: 1.3^niv 
    // -> utilisé pour le cout en mana d'un monstre non-tué
} Mana;


typedef struct {
    Gemme inventaire[GEMME_INV_MAX];    /* tableau des gemmes de l'inventaire */
    Bittab contient_gemme;              /* tableau de 1 si il y a une gemme 0 sinon */
    Mana jauge;                         /* mana du joueur */
} Joueur;

/**
 * @brief Initialise la jauge de mana avec les valeurs par défaut
 *
 * @param m mana à initialiser
 */
void init_mana(Mana *m);

/**
 * @brief Initialise un nouveau joueur avec les valeurs par défaut
 *
 * @param j joueur à initialiser
 */
void init_joueur(Joueur *j);

/**
 * @brief Renvoie l'indice de la première place libre dans l'inventaire
 * 
 * @param j joueur
 * @return int indice de la place libre dans inventaire, -1 sinon
 */
int gemme_libre(const Joueur *j);

/**
 * @brief Calcule le coût pour agrandir la réserve
 * 
 * @param qte_max quantité maximale de mana
 * @return int coût en mana calculé
 */
int cout_agrandir_mana(int qte_max);

/**
 * @brief Créée une nouvelle gemme et la stocke dans l'inventaire
 * 
 * @param j joueur
 * @param niv niveau de la gemme à créer
 * @return int 0 si la gemme a été créée \n
 * 1 si c'était impossible \n
 */
int joueur_creer_gemme(Joueur *j, int niv);

/**
 * @brief Echange deux gemmes dans l'inventaire
 * 
 * @param j joueur
 * @param gem1 indice de la première gemme
 * @param gem2 indice de la deuxième gemme
 * @return int 0 si l'échange a été fait \n 
 * 1 si c'était impossible \n
 */
int joueur_bouger_gemme(Joueur *j, int gem1, int gem2);

/**
 * @brief Fusionne les gemmes numéro gem1 et gem2 de l'inventaire
 * 
 * @param j joueur
 * @param gem1 indice de la première gemme
 * @param gem2 indice de la deuxième gemme
 * @return int 0 si la fusion a été faite \n 
 * 1 si c'était impossible \n
 */
int joueur_fusionner_gemmes(Joueur *j, int gem1, int gem2);

/**
 * @brief Aggrandis la réserve de mana
 * 
 * @param j joueur
 * @return int 0 si la réserve a été aggrandi \n 
 * 1 si c'était impossible \n
 */
int joueur_agrandir_reserve(Joueur *j);

/**
 * @brief Ajoute le mana gagné en tuant un monstre
 * 
 * @param j Joueur
 * @param pv_init pv initiaux du monstre
 */
void ajout_mana_monstre(Joueur *j, float pv_init);

/**
 * @brief Retire le coût en mana d'un monstre qui arrive au camp
 * 
 * @param j joueur
 * @param pv pv initiaux du monstre
 * @return int 0 si tout va bien \n
 * 1 si le joueur n'a plus assez de mana
 */
int retirer_mana_monstre(Joueur *j, float pv);

/**
 * @brief Créee une nouvelle tour sur une case
 * 
 * @param j joueur
 * @param terrain le terrain où il faut bâtir la tour
 * @param click case où bâtir la tour
 * @return int 0 si c'était possible \n 
 * 1 sinon
 */
int joueur_batire_tour(Joueur *j, Terrain *terrain, const Coord *click);

/**
 * @brief Ajoute la gemme d'indice indice_g dans la tour ou échange avec une gemme de l'inventaire
 * 
 * @param j joueur
 * @param terrain le terrain à modifier
 * @param indice_g indice de la gemme dans inventaire
 * @param click case où poser la gemme
 * @return int 0 si c'était possible \n 
 * 1 sinon 
 */
int joueur_poser_gemme(Joueur *j, int indice_g, Terrain *terrain, const Coord *click);

/**
 * @brief Retire la gemme de la tour et la met dans l'inventaire
 * 
 * @param j Joueur
 * @param terrain le terrain à modifier
 * @param indice_g indice de la gemme dans inventaire
 * @param tour_sel coordonées de la tour dans le terrain
 * @return int  0 si c'était possible \n
 * 1 sinon \n
 */
int joueur_retirer_gemme(Joueur *j, int indice_g, Terrain *terrain, const Coord *tour_sel);

#endif