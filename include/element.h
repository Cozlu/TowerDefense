#ifndef ELEM_H
#define ELEM_H

#include <time.h>
#include <math.h>

/**
 * @file element.h
 * @brief Permet d'utiliser les effets élémentaire et les éléments
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/* un temps interprété en millisecondes*/
typedef time_t time_ms;

/* un temps interprété en secondes*/
typedef time_t time_s; 

/* couleur entre 0 et 360 */
typedef int Teinte; 

typedef enum {
    MIXTE = 0,
    PYRO = 1,
    DENDRO = 3,
    HYDRO = 5,
    
    NO_EFFECT = 0,
    VAPO = 6,       // PYRO + HYDRO
    COMBUST = 4,    // PYRO + DENDRO
    ENRACIN = 8,    // DENDRO + HYDRO
} Element, Effet_Type;

typedef struct {
    Element residu;          /* résidu élémentaire */
    Effet_Type element;      /* effet actuellement au mosntre*/
    time_s fin_effet;        /* heure de fin de l'effet*/
    time_ms prochaine_maj;   /* heure (en ms) de la prochaine maj de l'effet */
    int degats;              /* degats pour effet poison */
} Effet;

/**
 * @brief Renvoie une chaîne représentant l'élément
 * 
 * @param elem Element à convertir
 * @return char* l'élement converti
 */
char *elem_to_string(Element elem);

/**
 * @brief Teste si la teinte est correcte
 *
 * @param teinte la teinte à tester
 * @return int 1 si elle est correcte et 0 sinon
 */
int teinte_est_correct(Teinte teinte);

/**
 * @brief Initialise un effet vide
 * 
 * @param effet effet à initialiser
 */
void init_effet_vide(Effet *effet);

/**
 * @brief Réalise la conversion d'une teinte entre 0 et 359 vers le système RGB
 * 
 * @param teinte la teinte à convertir
 * @param r la propriété rouge de la couleur
 * @param g la propriété verte de la couleur
 * @param b la propriété bleu de la couleur
 */
void teinte_vers_RGB(Teinte teinte, int *r, int *g, int *b); 

#endif