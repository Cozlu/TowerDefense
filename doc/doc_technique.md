BLONDEL Lucas - LEBLANC Travis

# Documentation technique Projet

## Introduction

Ce document sert à apporter des explications supplémentaires sur certains détails du code du projet pour pouvoir mieux le comprendre.

## Structures de données

### Terrain

Le terrain est un tableau à 2 dimensions de cases représentées par un enum. Ces cases peuvent contenir une donnée supplémentaire : si il s'agit d'une case avec tours ou avec chemin
Pour les chemins on stocke la direction de la prochaine case et de la case précédente. Dans l'idée cela s'apparente à une liste chaînée.

```c
typedef struct{
    Direction suiv, prec;
} Chemin;

typedef struct {
    TypeCase type;
    union{
        Chemin chemin;
        Tour tour;
    } donnee;
}Case;

typedef struct {
    int taille_chemin;                  /** nombre de case dans le chemin*/
    int nb_tours;                       /** nombre de tour dans le tableau*/
    Case carte[NB_LIGNES][NB_COLS];     /** tableau des cases du terrain*/
    Coord nid;
    Coord camp;
} Terrain;
```

### GenericTab

Etant donné que les structures de stockage pour les projectiles, monstres et animationsutilisent un tableau dynamique avec la même logique, nous avons implémenté un module de tableau générique : generictab. Ce tableau est alors doublé si la taille devient insuffisante.

```c
typedef struct {
    void *elems;      /** tableau des éléments*/
    size_t elem_size; /** tailles des éléments*/
    int nb_elem;      /** nombre d'élément actuellement dans le tableau*/
    int nb_max;       /** taille max du tableau (avant réalocation)*/
    char *name;       /** nom du tableau (pour debug)*/
} GenericTab;
```

De plus, chaque projectile pointe vers un monstre, donc les deux tableaux sont liés l'un à l'autre
(si on bouge un monstre on change aussi les projectiles) 


### Gemme
Les gemmes ne sont pas allouées dynamiquement car elles sont stockées soit dans l'inventaire du joueur qui a une taille fixe soit dans les tour du terrain.

```c
typedef struct {
    Teinte teinte;           /* couleur entre 0 et 360 */
    unsigned char niv;       /* niveau positif*/
    unsigned char pur;       /* 1 -> pur  0 -> mixte*/
} Gemme;
```

Les gemmes sont donc recopiées à chaque fois qu'elles sont bougées, ou qu'un projectile est crée.
Cela rend le code un peu moins efficace mais plus facile à lire et à écrire.

### Monstre

Nous avons décidé de stocker les monstres du jeu dans un seul tableau sans distinguer les vagues pour rendre les code plus efficace, facile à écrire et à lire.

```c
typedef struct {
    int num_vague;         /** num de la vague courante*/
    float pv_vague;        /** stocke les pv initiaux de la vague actuel == 1.2^num_vague*/
    GenericTab vague;      /** tableau des monstres sur le terrain*/
} TabVagues;
```

La structure monstre possède aussi un champ est_apparu pour nous permettre de savoir si le monstre doit être affiché sur le terrain ou non. Ce champ est utile dans le cas où les monstres apparaîssent à la suite depuis le nid.

### Bittab

Ce module nous permet de manipuler un entier non signé bit à bit comme un tableau. Il est notamment utilisé dans la structure de l'inventaire pour indiquer si une case contient une gemme ou non (1 oui et 0 non).

Bien qu'un tableau de char aurais pu être utilisé, nous avons préféré cette option pour optimiser encore plus le code.

```c
typedef struct {
    Gemme inventaire[GEMME_INV_MAX];    /* tableau des gemmes de l'inventaire */
    Bittab contient_gemme;              /* tableau de 1 si il y a une gemme 0 sinon */
    Mana jauge;                         /* mana du joueur */
} Joueur;
```

### Jeu

L'intéret de ce module est de nous permettre de créer une "interface" entre le moteur de jeu, la partie graphique et le main pour les manipuler plus aisément sans augmenter la taille du main.

## Algorithmes

Pour pouvoir gagner de la mana, il faut connaître les pv initiaux des monstres tués.
Notre solution est de propager la somme des pv des monstres tués lorsque l'on avance les projectiles pour qu'après tous les projectiles anvancer, on puisse gagner la mana nécéssaire (sans avoir à connaître les monstres individuellement)

Pour ce qui est de l'algorithme de génération de terrain, nous avons essayé d'appliquer au mieux celui du sujet dans la fonction construit_carte du module terrain. 

