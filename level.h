//
// Created by ethan on 19/10/2023.
//

#ifndef DOOMDEPTHSC_G21_LEVEL_H
#define DOOMDEPTHSC_G21_LEVEL_H

#include "Monster.h"

#define MAX_LVL 10

typedef struct {
    int lvl_number; // numéro du level.

    int nb_monster;

    st_monsters * p_monster;



} st_level;


int get_lvl(void);
int next_level(void);
int init_level(void);
st_monsters * get_lvl_monsters(int lvl);
int set_lvl_monsters(st_monsters * new_monster, int lvl);
int delete_all_level (void);

//st_level * create_level(int *p_nb_level);
//int display_level(st_level * p_first_level);

#endif //DOOMDEPTHSC_G21_LEVEL_H
