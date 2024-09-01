#include "../include/element.h"
#include <stdio.h>

void init_effet_vide(Effet *effet){
    effet->residu = NO_EFFECT;
    effet->element = NO_EFFECT;
    effet->degats = 0;
    effet->fin_effet = 0;
    effet->prochaine_maj = 0;
}

char *elem_to_string(Element elem){
    switch(elem){
        case PYRO: return "PYRO";
        case HYDRO: return "HYDRO";
        case DENDRO: return "DENDRO";
        case COMBUST: return "COMBUSTION";
        case ENRACIN: return "ENRACINEMENT";
        case VAPO: return "VAPORISATION";
        default: return "NO_ELEM";
    }
}

int teinte_est_correct(Teinte teinte){
    return 0 <= teinte && teinte <= 359;
}

void teinte_vers_RGB(Teinte teinte, int *r, int *g, int *b) {
    float r_tmp = 0;
    float g_tmp = 0; 
    float b_tmp = 0;
    float c = 1.0;
    float t_prime = teinte/60.0;
    float x = c*(1 - fabs(fmod(t_prime, 2) - 1));
    if (t_prime >= 0 && t_prime < 1) {
        r_tmp = c;
        g_tmp = x;
    } else if (t_prime < 2) {
        r_tmp = x;
        g_tmp = c;        
    } else if (t_prime < 3) {
        g_tmp = c;
        b_tmp = x;
    } else if (t_prime < 4) {
        g_tmp = x;
        b_tmp = c;
    } else if (t_prime < 5) {
        r_tmp = x;
        b_tmp = c;
    }  else {
        r_tmp = c;
        b_tmp = x;
    }
    *r = r_tmp * 255;
    *g = g_tmp * 255;
    *b = b_tmp * 255;
}