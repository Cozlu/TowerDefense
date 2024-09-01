#ifndef ANIM_H
#define ANIM_H

#include "element.h"
#include "generictab.h"

/**
 * @file animation.h
 * @brief Gère le stockage des animations lors des réactions élémentaires
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct{
    Element elem;
    float x,y;
    float rayon_max, rayon;
    time_ms fin;
} Animation;

typedef GenericTab TabAnimation; // le tableau des animations

/**
 * @brief Initialise un animation du jeu
 * 
 * @param anim une animation du jeu
 * @param elem le type de l'élément à animer
 * @param x la coordonée x de l'animation
 * @param y la coordonée y de l'animation
 * @param rayon le rayon de l'animation
 */
void init_animation(Animation *anim, Element elem, float x, float y, float rayon);

/**
 * @brief Met à jour une animation
 * 
 * @param anim l'animation à mettre à jour
 * @return int 0 si tout s'est bien passé
 */
int maj_animation(Animation *anim);

#endif
