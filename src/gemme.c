#include "../include/gemme.h"
#include "../include/constantes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/**
 * @brief Renvoie une teinte aléatoire pour une gemme pur d'élement spécifié
 * 
 * @param element élément pour choisir la teinte
 * @return Teinte la teinte aléatoire basée sur l'élément
 */
static Teinte random_teinte(Element element){
    int decalage;
    switch (element){
        case PYRO:
            decalage = 330;
            break;

        case DENDRO:
            decalage = 90;
            break;

        case HYDRO:
            decalage = 210;
            break;

        default:
            fprintf(stderr, "element %d inconnu\n", element);
            return -1;
    }
    return (decalage + rand()%60) % 360;
}


int gemme_egales(const Gemme *gem1, const Gemme *gem2){
    if(!gem1 || !gem2) return gem1 && gem2;
    return gem1->niv == gem2->niv && gem1->pur == gem2->pur && gem1->teinte == gem2->teinte;
}

Gemme init_gemme(Teinte teinte, unsigned char niv, unsigned char pur){
    Gemme g;
    g.teinte = teinte;
    g.niv = niv;
    g.pur = pur? 1 : 0;
    return g;
}

/**
 * @brief Renvoie un élément aléatoire
 *
 * @return Element élément tiré aléatoirement parmis PYRO, DENDRO et HYDRO
 */
static Element random_elem() {
    switch(rand() % 3) {
        case 0: return PYRO;
        case 1: return DENDRO;
        case 2: return HYDRO;
    }
    return PYRO;
}

Gemme random_gemme(int niv){
    Teinte teinte;
    Element element = random_elem();
    if(niv < 0 || (teinte = random_teinte(element)) < 0){
        fprintf(stderr, "gemme avec elem=%d niv=%d est illegale\n", element, niv);
        return GEMME_FAUSSE // paramètre faux
    }

    return init_gemme(teinte, niv, 1);
}

int prix_gemme(unsigned int niv){
    // 100 * 2^niv
    return 100 * (1 << niv);
}

Element element_gemme(const Gemme *g){
    if(!g) return -1;
    if(!g->pur) return MIXTE;
    
    if(abs(g->teinte - 120) <= 60)
        return DENDRO;
    
    if(abs(g->teinte - 240) <= 60)
        return HYDRO;
    
    if(abs((g->teinte + 30) % 360) <= 60)
        return PYRO;
    
    return MIXTE;
}

time_ms delai_tir_gemme(unsigned int niv){
    return CONSTANTE_DELAI_TIR*(650/((float)niv/10 + 1) + 100);
}

float distance_detection_gemme(unsigned int niv){
    return niv*0.3 + 3;
}

int peuvent_fusionner(const Gemme *gem1, const Gemme *gem2){
    return gem1 && gem2 && gem1->niv == gem2->niv;
}

/**
 * @brief Affiche la gemme sur la sortie standard
 *
 * @param g la gemme à afficher
 */
static void print_gemme(const Gemme *g){
    if(!g){
        fprintf(stdout, "Gemme: NULL\n");
        return;
    }
    fprintf(stdout, "Gemme: teinte=%d | niveau=%d | elem=%s\n", g->teinte, g->niv, elem_to_string(element_gemme(g)));
}

Gemme fusion_gemmes(const Gemme *gem1, const Gemme *gem2){
    int tmp1, tmp2;
    Gemme result;
    if(!peuvent_fusionner(gem1, gem2)){
        fprintf(stdout, "attention les gemmes:\n");
        print_gemme(gem1);
        print_gemme(gem2);
        fprintf(stdout, "ne peuvent pas fusionner\n");
        return GEMME_FAUSSE;
    }
    result.pur = gem1->pur && gem2->pur && (element_gemme(gem1) == element_gemme(gem2));
    result.niv = gem1->niv + 1;

    // si l'angle est obtus, on doit calculer le milieu sur l'angle aigue
    // donc on décale, on calcul le milieu, et on redecale comme avant(180 + 180 = 360)
    if(abs(gem1->teinte - gem2->teinte) >= 180){
        tmp1 = (gem1->teinte + 180) % 360;
        tmp2 = (gem2->teinte + 180 )% 360;
        result.teinte = ((tmp1 + tmp2)/2 + 180) % 360;
    }
    else{
        result.teinte = ((gem1->teinte + gem2->teinte)/2) % 360;
    }
    return result;
}
