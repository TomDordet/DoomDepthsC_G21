//
// Created by ethan on 19/10/2023.
//
#include "level.h"

st_level g_st_level[MAX_LVL]; // tableau de st_lvl, de taille 10 (donc 10 LVL créer). Faudras varier le nb de lvl.
static int g_current_lvl = 1; // lvl actuel.

//------------ CREATION LVL ----------------

// prend en param un lvl (struct), et le nombre max de lvl que l'on veut.
static int create_level( int id_db)
{

    memset (g_st_level, 0, sizeof (st_level) * MAX_LVL);

    srand(time(NULL) + rand());
    printf("DEBUG --- %s() ----id_db %d \n", __FUNCTION__ ,id_db);
    if (id_db == 0)
    {
        // Default value, Hardcoded Value.
        int max_monsters = 4;
        int min_monsters = 1;
        for (int i = 0; i < MAX_LVL; i++)
        {
            g_st_level[i].lvl_number = i + 1; // lvl_number ++
            g_st_level[i].nb_monster = rand() % (max_monsters - min_monsters + 1) + min_monsters;
            printf("DEBUG :: level %d\n", g_st_level[i].lvl_number); // debug
            printf("DEBUG :: nombre de monstres : %d\n", g_st_level[i].nb_monster);
            g_st_level[i].p_monster = create_monsters(g_st_level[i].nb_monster, g_st_level[i].lvl_number,  id_db); // création du monstre.
        }
        g_current_lvl = 1; // lvl actuel = 1
    }
    else
    {
        //Value from Data base
        // max_lvl --> le total de level sauve garder --> 2
        //   |-> lvl_number --> les levels sauvegarder --> 7 and 8
        //   |-> nb_monster --> nb de monstre pour chaque level
        //      |-> attack
        //      |-> maxLife
        //      |-> currentLife
        //      |-> defense
        int max_lvl = 2; //DB
        for (int i = 0; i < max_lvl; i++)
        {
            g_st_level[i].lvl_number = i + 1; //DB
            g_st_level[i].nb_monster = 2; //DB
            g_st_level[i].p_monster = create_monsters(g_st_level[i].nb_monster, g_st_level[i].lvl_number,  id_db); // création du monstre.
        }
        g_current_lvl = 1; // le plus petit lvl_number
    }

    return g_current_lvl;
}

//create lvl, mais sans avoir les paramètre à renseigner, pck le joueur est pas censé définri le nb de lvl tout ca.
int init_level(int id_db)
{
    printf("DEBUG --- %s() ----id_db %d \n", __FUNCTION__ ,id_db);
    return create_level( id_db); //création du lvl.
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

st_level * getLvl()
{
    return g_st_level;
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
        if (g_st_level[i].p_monster != NULL) {
            printf("DEBUG :: Delete level %d \n",g_st_level[i].lvl_number);
            delete_all_monster(g_st_level[i].p_monster);
        }

        g_st_level[i].nb_monster = 0;
    }
    reset_monster_number(); //on remet le nombre de monstre à 1.
    return 0;
}

