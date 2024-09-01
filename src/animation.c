#include "../include/constantes.h"
#include "../include/animation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void init_animation(Animation *anim, Element elem, float x, float y, float rayon){
    anim->elem = elem;
    anim->x = x;
    anim->y = y;
    anim->rayon_max = rayon;
    anim->rayon = rayon/FPS;
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    anim->fin = t.tv_sec*1000 + t.tv_nsec/1.0E6 + TEMPS_ANIMATION;
}

int maj_animation(Animation *anim){
    anim->rayon += anim->rayon_max*1000/(TEMPS_ANIMATION*FPS);
    if(anim->rayon > anim->rayon_max)
        anim->rayon = anim->rayon_max;
    return 0;
}
