#ifndef GRAPHISME_H
#define GRAPHISME_H

#include "constantes.h"
#include "element.h"
#include "tour.h"
#include "terrain.h"
#include "projectile.h"
#include "monstre.h"
#include "joueur.h"
#include "action.h"
#include "jeu.h"

/**
 * @file graphisme.h
 * @brief Affichage du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/**
 * @brief Affiche sur la fenêtre graphique le tower defense
 * 
 * @param jeu le jeu à afficher
 */
void dessine_jeu(const Jeu *jeu);

/**
 * @brief Affiche sur la fenêtre graphique l'écran de fin de partie
 * 
 * @param nb_degats le nombre de dégâts totaux infligés durant la partie
 * @param nb_vagues le nombre de vagues endurées
 */
void dessine_ecran_fin(int nb_degats, int nb_vagues);

#endif