#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "monstre.h"
#include "element.h"
#include "gemme.h"
#include "tour.h"
#include "generictab.h"
#include "animation.h"

/**
 * @file projectile.h
 * @brief Gère les projectiles du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    Gemme gemme;       /* gemme qui l'a lancé */
    float x, y;        /* position réel */
    Monstre *cible;    /* monstre vers lequel le projectile se dirige */
} Projectile;

typedef struct {
    GenericTab proj;
    int somme_degats;  /* somme des dégâts infligé par tous les projectiles */
    int somme_mort;    /* somme du nombre de monstre tué */
}TabProj;


/**
 * @brief Calcule la distance entre le projectile et sa cible
 * 
 * @param proj projectile
 * @return float distance avec le monstre 
 */
float distance_cible(const Projectile *proj);

/**
 * @brief Avance chaque projectile de la liste, fais exploser ceux qui sont arrivés
 * 
 * @details si le monstre meurt, tous les projectiles qui le vise disparaissent
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 * @param tab_anim tableau des animations
 * @return int la somme des pv des monstres morts
 */
int avance_tab_proj(TabProj *tab_proj, TabVagues *tab_vagues, TabAnimation *tab_anim);

/**
 * @brief Supprime le monstre du tableau et met à jour les projectiles en conséquence
 * 
 * @details les probjectiles qui cible le monstre ont leur cible mise à NULL
 * @param cible_mort monstre à supprimer
 * @param tab_proj tableau des projectiles
 * @param tab_vagues tableau des monstres
 */
void suppr_monstre_et_proj(Monstre *cible_mort, TabProj *tab_proj, TabVagues *tab_vagues);

/**
 * @brief Initialise un nouveau projectile de la tour vers le monstre
 * 
 * @param tab_proj tableau des projectiles
 * @param tour tour qui lance le projectile
 * @param li ligne de la tour
 * @param col colonne de la tour
 * @param monstr monstre vers lequel on tire
 * @return int 1 en cas d'erreur d'allocation 0 sinon
 */
int nouveau_projectile(TabProj *tab_proj, const Tour* tour, int li, int col, Monstre *monstr);

/**
 * @brief Décale toutes les cibles des projectiles de décalage
 * @warning on utilise cette fonction en cas d'un realloc du tableau de monstre à utiliser avec précaution
 * 
 * @param tab_proj tableau des projectiles
 * @param decalage décalage des pointeurs
 */
void decaler_cibles_proj(TabProj *tab_proj, size_t decalage);

/**
 * @brief Parcours les tours et les monstres pour créer tous les projectiles qu'il faut
 * @details les projectiles ne sont créés que si le delai de la gemme est passé
 * 
 * @param tab_proj tableau des projectiles
 * @param terrain état courant du terrain
 * @param tab_vague tableau des monstres
 * @param cur_time heure actuelle
 * @return int nombre de projectiles créés -1 si problème d'allocation
 */
int all_nouveaux_projectiles(TabProj *tab_proj, Terrain *terrain, const TabVagues *tab_vague, struct timespec cur_time);

/**
 * @brief Initialise le tableau des projectiles
 *
 * @param tab_proj tableau des projectiles
 * @return int 1 en cas d'erreur d'allocation, 0 si tout va bien
 */
int init_tab_proj(TabProj *tab_proj);

/**
 * @brief Libère la mémoire allouée au tableau des projectiles
 *
 * @param tab_proj tableau des projectiles à libérer
 */
void libere_tab_projectile(TabProj *tab_proj);

#endif