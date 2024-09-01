#include "../include/generictab.h"
#include <stdio.h>
#include <string.h>

void print_GenericTab(const GenericTab *tab, void (*print_elem)(const void *elem)){
    printf("[");
    for(int i = 0; i < tab->nb_elem - 1; i++){
        print_elem(get_elem(tab, i));
        printf(", ");
    }
    if(tab->nb_elem >= 1)
        print_elem(get_elem(tab, tab->nb_elem - 1));
    printf("]\n");
}

int init_GenericTab(GenericTab *tab, size_t elem_size, int nb_max){
    if(!(tab->elems = malloc(elem_size*nb_max))){
        fprintf(stderr, "erreur d'allocation de %d elements de taille %ld\n", nb_max, elem_size);
        return 1;
    }
    tab->elem_size = elem_size;
    tab->nb_max = nb_max;
    tab->nb_elem = 0;
    tab->name = "tab";
    return 0;
}

int correct_index(const GenericTab *tab, int i){
    return 0 <= i && i < tab->nb_elem;
}

void *get_elem(const GenericTab *tab, int i){
    if(!correct_index(tab, i)){
        fprintf(stderr, "%s[%d] out of bounds\n", tab->name, i);
        return NULL;
    }
    return ((char*)tab->elems) + i*tab->elem_size;
}

void *get_last_elem(const GenericTab *tab){
    return get_elem(tab, tab->nb_elem - 1);
}

void *get_first_elem(const GenericTab *tab){
    return get_elem(tab, 0);
}

/**
 * @brief Donne l'élément du tableau à l'indice i (équiv à tab[i])
 * 
 * @param tab tableau pour obtenir l'élément
 * @param i indice de l'élément
 * @return void* pointeur sur l'élément, NULL si l'indice n'est pas dans le tableau
 */
static void *get_elem_no_secure(const GenericTab *tab, int i){
    return ((char*)tab->elems) + i*tab->elem_size;
}

int add_elem(GenericTab *tab, void *elem){ 
    return set_elem(tab, tab->nb_elem, elem);
}

int set_elem(GenericTab *tab, int i, void *elem){
    void * tmp;
    if(i < 0 || i > tab->nb_elem){
        return 0;
    }
    if(i == tab->nb_elem && tab->nb_elem == tab->nb_max){
        if(!(tmp = realloc(tab->elems, tab->elem_size*tab->nb_max*2))){
            fprintf(stderr, "erreur de reallocation de %d element de taille %ld\n", tab->nb_max*2, tab->elem_size);
            return 1;
        }
        tab->elems = tmp;
        tab->nb_max *= 2;
    }
    memcpy(((char*)tab->elems) + i*tab->elem_size, elem, tab->elem_size);
    if(i == tab->nb_elem)
        tab->nb_elem++;
    return 0;
}

int echange_elem(GenericTab *tab, int i, int j){
    void *elem1, *elem2, *tmp;
    int alloc = 0;
    if(!((elem1 = get_elem(tab, i)) && (elem2 = get_elem(tab, j))))
        return -1; // un indice est faux
    
    if(tab->nb_elem < tab->nb_max){ // on prend une case de tableau comme variable temporaire
        tmp = get_elem_no_secure(tab, tab->nb_max - 1);
    }
    else {
        if(!(tmp = malloc(sizeof(tab->elem_size)))) // on a pas de place donc on alloue un element
            return 1;
        alloc = 1;
    }
    
    // tmp = tab[i]
    memcpy(tmp, elem1, tab->elem_size);
    // tab[i] = tab[j]
    memcpy(elem1, elem2, tab->elem_size);
    // tab[j] = tmp
    memcpy(elem2, tmp, tab->elem_size);

    if(alloc)
        free(tmp);
    
    return 0;
}

void *remove_elem(GenericTab *tab, int i){
    void *tmp;
    if(!correct_index(tab, i))
        return NULL;
    if(i != tab->nb_elem - 1){
        tmp = get_elem(tab, tab->nb_elem - 1);
        memcpy(get_elem(tab, i), tmp, tab->elem_size);
    }
    tab->nb_elem--;
    return tmp;
}

void for_each_elem(const GenericTab *tab, void (*f)(const void*)){
    for(int i = 0; i < tab->nb_elem; i++){
        f(get_elem(tab, i));
    }
}

int filter_elem(GenericTab *tab, int (*filter)(const void*)){
    int nb = 0;
    for(int i = tab->nb_elem - 1; i >= 0; i--){
        if(filter(get_elem(tab, i))){
            remove_elem(tab, i);
            nb++;
        }
    }
    return nb;
}

void free_GenericTab(GenericTab *tab){
    if(tab && tab->elems){
        free(tab->elems);
        tab->nb_elem = 0;
        tab->nb_max = 0;
    }
}
