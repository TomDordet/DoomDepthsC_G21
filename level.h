//
// Created by ethan on 19/10/2023.
//

#ifndef DOOMDEPTHSC_G21_LEVEL_H
#define DOOMDEPTHSC_G21_LEVEL_H

#include "Monster.h"

#define MAX_LVL 10

typedef struct
{
    int lvl_number; // numéro du level.

    int nb_monster;

    st_monsters * p_monster;

} st_level;

extern st_level g_st_level[MAX_LVL]; // tableau de st_lvl, de taille 10 (donc 10 LVL créer). Faudras varier le nb de lvl.

int get_lvl(void);
st_level * getLvl();
int next_level(void);
int init_level(int id_db);
st_monsters * get_lvl_monsters(int lvl);
int set_lvl_monsters(st_monsters * new_monster, int lvl);
int delete_all_level (void);

#endif //DOOMDEPTHSC_G21_LEVEL_H
