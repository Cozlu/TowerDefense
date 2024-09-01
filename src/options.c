#include "../include/options.h"
#include <stdio.h>

int usage(char * prog) {
    fprintf(stderr, "Mauvais usage : %s [OPTION]\n", prog);
    fprintf(stderr, "-h, --help : Affiche une aide pour utiliser le programme\n");
    return 2;
}

int help(char * prog) {
	fprintf(stdout,"----------------------------------------------------------------\n");
    fprintf(stdout, "Lance une partie de tower defense sur le modèle du jeu Gemcraft\n");
   	fprintf(stdout,"----------------------------------------------------------------\n");

	fprintf(stdout, "usage : %s [OPTIONS]\n", prog);
    fprintf(stdout, "-h, --help : Affiche une aide pour utiliser le programme\n\n");
    fprintf(stdout, "\
   exemple : \n\
   %s \n\
   %s --help\n", prog, prog);
   fprintf(stdout,"----------------------------------------------------------------\n");
    return 1;
}

int options(int argc, char* argv[]) {
    int opt;
    if (argc > 2) return 2;
    static struct option long_opt[] = {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };
    while((opt = getopt_long(argc, argv, "h", long_opt, NULL)) != -1) {
        switch(opt) {
            case 'h' : return 1;
            case '?' : return 2;
            default : return 2;
        }
    }
    return 0;
}