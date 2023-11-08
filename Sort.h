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
    MURDECLACE = 3,
    BOUCLIERDELUMIERE = 4,
    REGENERATIONVIE = 5,
    REGENERATIONMANA = 6,
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
void sort(st_player* p_player, st_monsters* p_monster, Sort* playerChoice);

#endif //DOOMDEPTHSC_G21_SORT_H
