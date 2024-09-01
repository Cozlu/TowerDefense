#include "../include/constantes.h"
#include "../include/options.h"
#include "../include/graphisme.h"
#include "../include/jeu.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <MLV/MLV_all.h>

int main(int argc, char *argv[]){
    /* Options du programme */
    switch(options(argc, argv)) {
        case 1 : return help(argv[0]); break;
        case 2 : return usage(argv[0]); break;
    }
    /* Programme principal */
    struct timespec end_time, new_time;
    double frametime, extratime;
    MLV_Input_box *gen_gemme;
    Jeu jeu;
    int result;

    #ifdef _TEST_
    srand(5);
    #else
    srand(time(NULL));
    #endif

    if (init_jeu(&jeu)) return 1; // erreur d'allocation
    
    #ifdef _TEST_
    jeu.joueur.jauge.qte_max = jeu.joueur.jauge.reserve = 20000;
    #endif

    MLV_create_window("Tower defense", "Tower defense", WIN_LARGEUR, WIN_HAUTEUR);
    
    gen_gemme = MLV_create_input_box(
        WIN_LARGEUR - 235 , 160, 200, 60,
        MLV_COLOR_WHITE, MLV_COLOR_WHITE,
        MLV_rgba(173, 24, 186, 255), "Générer gemme de niv : "
    );
    
    while(jeu.joueur.jauge.reserve > 0){
        clock_gettime(CLOCK_REALTIME, &end_time); 
        /* Actions du joueur */
        if((result = gere_evenements(&jeu, end_time))){
            libere_jeu(&jeu);
            MLV_free_input_box(gen_gemme);
            MLV_free_window();
            switch (result){
                case -1 : return 0; // action "quitter"
                default : return 1; // erreur d'allocation
            }
        }
        if(maj_jeu(&jeu, end_time)){
            libere_jeu(&jeu);
            MLV_free_input_box(gen_gemme);
            MLV_free_window();
            return 1; // erreur d'allocation
        }
        dessine_jeu(&jeu);
        /* Gestion du framerate */
        clock_gettime(CLOCK_REALTIME, &new_time);
        frametime = new_time.tv_sec - end_time.tv_sec;
        frametime += (new_time.tv_nsec - end_time.tv_nsec) / 1.0E9;

        extratime = 1.0/FPS - frametime;
        if(extratime > 0){
            MLV_wait_milliseconds((int) (extratime * 1000));
        }
    }
    /* Fin de partie */
    dessine_ecran_fin(jeu.projectiles.somme_degats, jeu.vagues.num_vague);
    libere_jeu(&jeu);
    MLV_wait_seconds(5);
    MLV_free_input_box(gen_gemme);
    MLV_free_window();
    return 0;
}
