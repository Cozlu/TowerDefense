#ifndef OPT_H
#define OPT_H

#include <getopt.h>

/**
 * @file options.h
 * @brief Gère les options du programme
 * @author Blondel Lucas
 * @author Leblanc Travis
 * @date novembre 2023 - janvier 2024
 */

/**
 * @brief Affiche et indique comment lancer correctement le programme
 *
 * @param prog le nom du programme
 * @return le code d'erreur 2
 */
int usage(char * prog);

/**
 * @brief Affiche une aide pour utiliser le programme
 *
 * @param prog le nom du programme
 * @return 1 (comportement normal)
 */
int help(char * prog);

/**
 * @brief Traite toutes les options entrées dans la ligne de commande
 *
 * @param argc le nombre d'arguments du programme
 * @param argv les arguments du programme
 * @return 1 si l'option help est présente, 2 si il y a un mauvais usage de la ligne de commande et 0 sinon
 */
int options(int argc, char* argv[]);

#endif
