#ifndef MONSTRE_H
#define MONSTRE_H
#include "constantes.h"
#include "element.h"
#include "terrain.h"
#include "generictab.h"

/**
 * @file monstre.h
 * @brief Gère les monstres du jeu
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

typedef struct {
    Teinte teinte;                /** couleur du monstre*/
    int est_apparu;               /** 1 si le monstre est apparu, 0 sinon*/
    float vitesse, vitesse_init;  /** vitesse en nb_case/s */
    float pv, pv_init;            /** pv du monstre*/
    float x, y;                   /** position réel*/
    Direction dir;                /** direction dans laquel se dirige le monstre*/
    Effet effet;                  /** effet elementaire*/
} Monstre;

typedef struct {
    int num_vague;         /** num de la vague courante*/
    float pv_vague;        /** stocke les pv initiaux de la vague actuel == 1.2^num_vague*/
    GenericTab vague;      /** tableau des monstres sur le terrain*/
} TabVagues;

typedef enum {
    NORMALE,
    FOULE,
    AGILE,
    BOSS
} TypeVague;

/**
 * @brief Initialise un monstre
 * 
 * @param teinte teinte du monstre
 * @param vitesse vitesse en case par secondes du monstre
 * @param pv vie initiale du monstre
 * @param x coordonnée x réelle du monstre
 * @param y coordonnée y réelle du monstre
 * @return Monstre le monstre initialisé
 */
Monstre init_monstre(Teinte teinte, float vitesse, float pv, float x, float y);

/**
 * @brief Initialise la tableau des vagues
 * 
 * @param tab_vagues pointeur vers le tableau à initialiser
 * @return int 0 si il n'y a pas eu d'erreur et 1 sinon
 */
int init_tab_vagues(TabVagues *tab_vagues);

/**
 * @brief Ajoute un monstre au tableau des vagues
 * 
 * @param tab_vagues pointeur vers le tableau des vagues
 * @param monstre monstre à ajouter
 * @return int 0 si il n'y a pas eu d'erreur d'allocation et 1 sinon
 */
int ajout_monstre(TabVagues *tab_vagues, Monstre monstre) ;

/**
 * @brief Applique les dégâts au monstre
 *
 * @param m monstre sur lequel appliquer les dégâts
 * @param degats dégâts à appliquer
 * @return int les dégâts réellement appliqué si le monstre est mort, 0 sinon
 */
int degats_sur_monstre(Monstre *m, int degats);

/**
 * @brief Avance le monstre selon la distance tirée
 * 
 * @param m pointeur vers le monstre à avancer
 * @param terrain terrain du jeu
 * @return int 1 si le monstre touche le camp et 0 sinon
 */
int avancer_monstre(Monstre * m, const Terrain *terrain);

/**
 * @brief Met à jour l'effet sur le monstre:
 * - le retire si le temps est passé 
 * - applique le poison s'il en a un 
 * 
 * @param m monstre à mettre à jour
 * @param cur_time heure actuelle
 * @return int 1 si le monstre est mort 0, sinon
 */
int maj_effet_monstre(Monstre *m, struct timespec cur_time);

/**
 * @brief Supprime un monstre du tableau des vagues
 * 
 * @param tab_vagues pointeur vers le tableau de vagues où il faut supprimer le monstre
 * @param i indice du monstre à supprimer
 */
void suppr_monstre(TabVagues *tab_vagues, int i);

/**
 * @brief Renvoie un pointeur vers le dernier monstre du tableau
 * 
 * @param tab_vagues pointeur vers le tableau des vagues
 * @return Monstre* pointeur vers le dernier monstre
 */
Monstre *dernier_monstre(const TabVagues *tab_vagues);

/**
 * @brief Libère la mémoire allouée au tableau des vagues et aux listes qu'elle contient
 * 
 * @param tab_vagues pointeur vers le tableau des vagues à libérer
 */
void libere_tab_vagues(TabVagues *tab_vagues);

/**
 * @brief Calcule la distance en nombre de cases entre le centre de deux monstres
 * 
 * @param m1 pointeur vers le 1er monstre
 * @param m2 pointeur vers le 2ème monstre
 * @return float la distance en cases entre deux monstres
 */
float distance_monstre(const Monstre *m1, const Monstre *m2);

/**
 * @brief Créée et ajoute une vague tirée aléatoirement dans le tableau
 * 
 * @param tab_vagues tableau des vagues de monstre
 * @param terrain terrain du jeu
 * @return 1 en cas d'erreur d'allocation 0 sinon
 */
int vague_aleatoire(TabVagues *vagues, Terrain *terrain);


#endif
