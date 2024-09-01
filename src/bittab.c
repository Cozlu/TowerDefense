#include "../include/bittab.h"
#include <stdio.h>

Bittab init_Bittab(){
    return 0;
}

char get_bit(const Bittab *tab, unsigned int i){
    return (*tab >> i) & 1; // on prend le ième bit
}

void set_bit(Bittab *tab, unsigned int i, char value){
    if(value)
        *tab |= 1 << i;
    else
        *tab &= ~(1 << i);
}