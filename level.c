//
// Created by ethan on 19/10/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "level.h"

static st_level g_st_level[MAX_LVL]; // tableau de st_lvl, de taille 10 (donc 10 LVL créer). Faudras varier le nb de lvl.
static int g_current_lvl = 0; // lvl actuel.

//------------ CREATION LVL ----------------

// prend en param un lvl (struct), et le nombre max de lvl que l'on veut.
static int create_level(st_level * p_st_lvl, int max_lvl)
{
    for (int i = 0; i < max_lvl; i++)
    {
        p_st_lvl[i].lvl_number = i + 1; // lvl_number ++
        printf("debug :: level %d \n", p_st_lvl[i].lvl_number); // debug
        p_st_lvl[i].p_monster = create_monsters(&p_st_lvl[i].nb_monster,  p_st_lvl[i].lvl_number ); // création du monstre.

    }
    g_current_lvl = 1; // lvl actuel = 1
    return 0;
}

//create lvl, mais sans avoir les paramètre à renseigner, pck le joueur est pas censé définri le nb de lvl tout ca.
int init_level(void)
{
    return create_level(g_st_level, MAX_LVL); //création du lvl.
}

//-------------- SET MONSTERS by LVL -----------

//création des monstres par niveau.
int set_lvl_monsters(st_monsters * new_monster, int lvl)
{
    for (int i = 0; i < MAX_LVL; i ++)
    {
        // on parcours les lvl, si on tombe sur le lvl sur lequel on est (donc celui passer en para) ;
        if (g_st_level[i].lvl_number == lvl)
        {
            g_st_level[i].p_monster = new_monster; // alors l'attribut p_monster (qui est une st de monstre dans la st du lvl) devient un monstre.
            return g_st_level[i].lvl_number;
        }
    }
    return 0;
}

//-------------- GET INFO --------------------

//get le lvl actuel.
int get_lvl(void)
{
    return g_current_lvl;
}

// permet d'aller au niv suivant.
int next_level(void)
{
    g_current_lvl++;
    return g_current_lvl;
}

//retourne les monstres du lvl
st_monsters * get_lvl_monsters(int lvl)
{
    for (int i = 0; i < MAX_LVL; i ++)
    {
        if (g_st_level[i].lvl_number == lvl)
            return g_st_level[i].p_monster;
    }
    return NULL;
}

//delete les lvl (et tout les monstres qu'il y'a dedans).
int delete_all_level (void)
{
    for (int i = 0; i < MAX_LVL; i ++)
    {
        printf("Delete level %d \n",g_st_level[i].lvl_number);
        if (g_st_level[i].p_monster != NULL) {
            delete_all_monster(g_st_level[i].p_monster);
        }
        else
        {
            printf("no monster..... \n");
        }
        g_st_level[i].nb_monster = 0;
    }
    reset_monster_number(); //on remet le nombre de monstre à 1.
    return 0;
}

