#ifndef DOOMDEPTHSC_G21_PLAYER_H
#define DOOMDEPTHSC_G21_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Weapon.h"
#include "Armor.h"

typedef struct st_player
{
    int maxLife; //stats player
    int currentLife;
    int maxMana;
    int currentMana;
    int attack;
    int defense;
    int gold;
    WeaponsPlayer *weapons;
    ArmorsPlayer *armors;
} st_player;

st_player *create_player(int id_db);

st_player *delete_player(st_player *p_player);

int display_player(st_player *p_player);

#endif //DOOMDEPTHSC_G21_PLAYER_H
