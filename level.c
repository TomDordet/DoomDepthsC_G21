//
// Created by ethan on 19/10/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "level.h"
#include "sqlite3.h"

st_level g_st_level[MAX_LVL]; // tableau de st_lvl, de taille 10 (donc 10 LVL créer). Faudras varier le nb de lvl.
static int g_current_lvl = 1; // lvl actuel.

//------------ CREATION LVL ----------------

// prend en param un lvl (struct), et le nombre max de lvl que l'on veut.
static int create_level(int save_id)
{
    // REMPLIE LA ZONE MEMOIRE
    memset (g_st_level, 0, sizeof (st_level) * MAX_LVL);
    // rand pour aléatoire
    srand(time(NULL) + rand());
    printf("DEBUG --- %s() ----id_db %d \n", __FUNCTION__ ,save_id);
    // si save id == 0, c'est que aucune sauvegarde n'a été selectionner, donc on commence une partie normal.
    if (save_id == 0)
    {
        // Default value, Hardcoded Value.
        int max_monsters = 4;
        int min_monsters = 1;
        // on parcours tout les niveaux.
        for (int i = 0; i < MAX_LVL; i++)
        {
            g_st_level[i].lvl_number = i + 1; // lvl_number ++
            g_st_level[i].nb_monster = rand() % (max_monsters - min_monsters + 1) + min_monsters; // as besoin d'un nombre de monstres
            printf("DEBUG :: level %d\n", g_st_level[i].lvl_number); // debug
            printf("DEBUG :: nombre de monstres : %d\n", g_st_level[i].nb_monster);
            g_st_level[i].p_monster = create_monsters(g_st_level[i].nb_monster, g_st_level[i].lvl_number); // création du monstre.
        }
        g_current_lvl = 1; // lvl mini
    }
    else // si une sauvegarde a été selectionner
    {
        // BDD
        int rc = 0;
        sqlite3 *db;
        rc = sqlite3_open("D:/Bilal/Code/DoomDepths/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
        if (rc)
        {
            fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        }

        // LA REQUETE POUR REQUETE DES INFOS SUR LE NIVEAU
        const char *select_minMaxLvl_sql = "SELECT MAX(lvl_number), COUNT(*), MIN(lvl_number) FROM LEVEL WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, select_minMaxLvl_sql, -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        // BIND LES ZONES INCONNUES AVEC DES VARIABLES
        sqlite3_bind_int(stmt, 1, save_id);

        // ON STOCK LES VALEURS RECUPERES DANS DES VARIABLES
        rc = sqlite3_step(stmt);
        int nbLvl = sqlite3_column_int(stmt, 0);
        int allLvl = sqlite3_column_int(stmt, 1);
        int minLvl = sqlite3_column_int(stmt, 2);

        //FINALISE LA TRANSACTION
        sqlite3_finalize(stmt);

        // NOUVELLE REQUETE POUR RECUP LES DATA
        const char *select_levels_sql = "SELECT lvl_number, nb_monster, p_monster FROM LEVEL WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt2;
        rc = sqlite3_prepare_v2(db, select_levels_sql, -1, &stmt2, 0);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
            return rc;
        }

        sqlite3_bind_int(stmt2, 1, save_id);

        int levelIndex = minLvl;
        // TANT QUIL YA DES COLONNES && TANT QUE I < au nombre de level. (donc il parcours tout les lvl)
        int previousLevel = -1;
        while ((rc = sqlite3_step(stmt2)) == SQLITE_ROW && levelIndex < allLvl)
        {
            // ON RECUP LES DATA
            int lvlNumber = sqlite3_column_int(stmt2, 0);
            int nbMonster = sqlite3_column_int(stmt2, 1);

            if (lvlNumber != previousLevel)
            {
                printf("NUMBER LEVEL : %d\n", lvlNumber);
                previousLevel = lvlNumber;  // Mettez à jour le niveau précédent
                printf("NB MONSTRES DANS LE LVL : %d\n", nbMonster);

                for(int j = 0; j < nbMonster; j++)
                {
                    int num_monster = sqlite3_column_int(stmt2, 2);
                    printf("NUMERO DU MONSTRE TAVU %d\n", num_monster);

                    g_st_level[levelIndex].lvl_number = lvlNumber; //DB
                    g_st_level[levelIndex].nb_monster = nbMonster; //DB

                    g_st_level[levelIndex].p_monster = create_monsters_save(g_st_level[levelIndex].p_monster,save_id, num_monster);


                    if(j < (nbMonster-1))
                        rc = sqlite3_step(stmt2);
                }
            }
            levelIndex++; // Incrémentez le compteur
        }
        sqlite3_finalize(stmt2);
        sqlite3_close(db);

        printf("min LVL : %d\n", minLvl);
        printf("nb LVL : %d\n", nbLvl);

        g_current_lvl = minLvl; // le plus petit lvl_number
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
        // on parcourt les lvl, si on tombe sur le lvl sur lequel on est ;
        if (g_st_level[i].lvl_number == lvl)
        {
            g_st_level[i].p_monster = new_monster; // alors l'attribut p_monster (qui est une struct de monstre dans la struct du lvl) devient un monstre. On init les monstres en gros. (si NULL passer en param, alors pu de monstres).
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
    // parcous les lvls
    for (int i = 0; i < MAX_LVL; i ++)
    {
        // retourne le monstre du level.
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

