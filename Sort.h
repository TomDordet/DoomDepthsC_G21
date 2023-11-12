#ifndef DOOMDEPTHSC_G21_SORT_H
#define DOOMDEPTHSC_G21_SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "Monster.h"

enum Sorts {
    BOULEDEFEU = 1,
    ECLAIRFULGURANT = 2,
    DECUPLEMENT = 3,
    MURDECLACE = 4,
    BOUCLIERDELUMIERE = 5,
    REGENERATIONVIE = 6,
    REGENERATIONMANA = 7,
};

enum SortsTypes {
    OFFENSIVE,
    DEFENSIVE,
    LIFEHEAL,
    MANAHEAL,
};

typedef struct
{
    enum Sorts sort;
    int damage;
    int resources;
    enum SortsTypes type;

} Sort;


/*cette fonction lance le sort player choice lancé par p_player contre p_monster*/
void sort(st_player* p_player, st_monsters* p_monster, Sort playerChoice);

/*Fonction pour afficher chaque sort et leurs descriptions*/
void afficherDescriptionSort(enum Sorts sort);


#endif //DOOMDEPTHSC_G21_SORT_H
