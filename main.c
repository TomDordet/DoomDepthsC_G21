#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"
#include "sqlite3.h"



// PROTOTYPE FONCTIONS (pas issue de .h donc on les protos ici)
// FIGHT
st_monsters *fight_player_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func joueur attaque monstre
st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func monstre(s) attaque joueur


//HEAL
int heal(st_player *p_player); //proto heal

// INSERT DATA (SAVE)
int insertTheSAVE(sqlite3 *db, st_player* p_player, st_level* p_level);

// LOAD SAVE
int displayLatestSaves(void);

// --------------- BDD -----------------------------------

// create_table prend en param la db
int create_tables(sqlite3 *db)
{

    const char *create_joueur_table_sql =
            "CREATE TABLE JOUEUR ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "maxLife INTEGER, "
            "currentLife INTEGER, "
            "attack INTEGER, "
            "id_niveau INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (id_niveau) REFERENCES LEVEL(lvl_number), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_monstre_table_sql =
            "CREATE TABLE MONSTRE ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "number INTEGER, "
            "maxLife INTEGER, "
            "currentLife INTEGER, "
            "attack INTEGER, "
            "p_next INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (p_next) REFERENCES MONSTRE(id), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_level_table_sql =
            "CREATE TABLE LEVEL ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "lvl_number INTEGER, "
            "nb_monster INTEGER, "
            "p_monster INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (p_monster) REFERENCES MONSTRE(id), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_sauvegarde_table_sql =
            "CREATE TABLE SAUVEGARDE ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "currentLevel INTEGER, "
            "date TEXT "
            ");";
    /*
    const char *create_level_monstre_table_sql =
            "CREATE TABLE LEVEL_MONSTER ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "id_niveau INTEGER, "
            "id_monstre INTEGER, "
            "FOREIGN KEY (id_niveau) REFERENCES LEVEL(lvl_number), "
            "FOREIGN KEY (id_monstre) REFERENCES MONSTRE(id)"
            ");";

    */
    /*
    int rc = 0;

    const char * drop_joueur_sql = "DROP TABLE JOUEUR;";
    const char * drop_monster_sql = "DROP TABLE MONSTRE;";
    const char * drop_level_sql = "DROP TABLE LEVEL;";
    const char * drop_sauvegarde_sql = "DROP TABLE SAUVEGARDE;";

    rc = sqlite3_exec(db, drop_joueur_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_monster_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_level_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_sauvegarde_sql, 0, 0, 0);
    */


    int rc = 0;

    rc = sqlite3_exec(db, create_joueur_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_monstre_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_level_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_sauvegarde_table_sql, 0, 0, 0);

    return 0;
}

// Création / ouverture de la base de données
int init_bdd(void)
{
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Créez les tables
    rc = create_tables(db);
    if (rc != 0) {
        fprintf(stderr, "Erreur lors de la création des tables.\n");
    }
    sqlite3_close(db);
    return 0;
}

int insertData(st_player* p_player, st_level* p_level)
{
    fprintf(stderr, "Début de la fonction insertData\n");
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    insertTheSAVE(db, p_player, p_level);

    sqlite3_close(db);
    fprintf(stderr, "Fin de la fonction insertData\n");
    return 0;
}


// --------------- DISPLAY ENTRE LVL ---------------------

int first_menu(void)
{
    while(1)
    {
        int save_choice = 1;
        printf("Quelques minutes de pause avant de reprendre... Que voulez vous faire ? \n");
        printf("1 - Inventaire\n");
        printf("2 - Sauvegarde\n");
        printf("3 - Continuer\n");
        printf("4 - Exit\n");

        scanf("%d", &save_choice);

        switch(save_choice){
            case 1: {
                printf("inventaire à venir..\n");
                break;
            }
            case 2:
                return 2;
            case 3:
                return 3;
            case 4:
                return 4;
        }
    }
}

// ---- LA GAME

//fin de game
int exit_game (st_player *p_player)
{
    //Libere tous les montres
    delete_all_level ();
    //libere le joueur
    delete_player(p_player);
    return 0;
}

int game(int id_db)
{
    int choixMenu = 1;
    printf("DEBUG --- %s ----id_db %d \n", __FUNCTION__ ,id_db);
    st_player *p_player = create_player(id_db); // on créer le joueur.
    init_level(id_db); // init le lvl (qui init la créa des mstr)

    while (1)
    {
        printf("--- LEVEL %d ----\n", get_lvl());
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.heal \n");
        printf("5.sortir \n");
        scanf("%d", &choixMenu);

        switch (choixMenu)
        {
            case 1: {
                display_player(p_player); // affiche les stats du joueur.
                break;
            }
            case 2:
            {
                printf("-- Level %d -- \n",get_lvl()); // affiche le LVL actuel
                display_monsters(get_lvl_monsters(get_lvl())); // affiche les monstres.
                break;
            }
            case 3:
            {
                /*Game*/
                st_monsters * p_fight = fight_player_round(p_player, get_lvl_monsters(get_lvl()));
                // Si le retour de fight_player_round == NULL, alors monstres morts / désallouer (voir ce que la fonction retourne à sa définition, pour comprendre).
                if (p_fight == NULL)
                {
                    int f_menu;

                    // on remet à NULL les monstres dans le lvl, pck ils sont morts.
                    set_lvl_monsters(NULL, get_lvl());
                    printf("Tout les monstres sont morts !");
                    f_menu = first_menu();
                    if (f_menu == 2)
                    {
                        insertData(p_player, g_st_level);
                    }
                    else if (f_menu == 3)
                    {//next game, next level
                        //next_game();
                        next_level();
                    }
                    else if(f_menu == 4)//Exit
                    {
                        return exit_game (p_player);
                    }
                    break;
                }
                else if (p_fight != get_lvl_monsters(get_lvl()))
                {
                    set_lvl_monsters(p_fight, get_lvl());
                }
                else
                {
                    //pNew == get_lvl_monsters(get_lvl()
                    //donc on fait rien
                }

                p_player = fight_monsters_round(p_player, get_lvl_monsters(get_lvl())); // déclenche l'attaque du monstre.
                if (NULL == p_player) // si joueur = NULL (donc mort...)
                {
                    printf("Player is dead (has been deleted) .. Game over !!!!! \n ");
                    /*Libere tous les montres*/
                    delete_all_level ();
                    return 0; // ?
                }
                break;
            }
            case 4:
                heal(p_player);
                break;
            case 5:
            {
                int var;
                printf("Tout sera supprimer, continuer ? (1/0)\n");
                scanf("%d", &var);
                if (var == 1)
                {
                    return exit_game (p_player);
                }
                break;
            }
        }
    }


    //system("PAUSE");
    return 0;
}
// ---------------- WELCOME ----------------

int welcome(void)
{
    int choixMenu = 0;
    int id_db = 0;
    while (choixMenu != 3)
    {
        printf("--DOOMDEPTH-- \n\n");
        printf("1 - Start Game \n");
        printf("2 - Load save \n");
        printf("3 - Exit\n");
        scanf("%d", &choixMenu);

        switch (choixMenu) {
            case 1: {
                game(id_db);
                break;
            }
            case 2:
            {
                id_db = displayLatestSaves();
                printf("ID DB is %d\n", id_db);
                break;
            }
            case 3: {
                break;
            }
        }
    }
    return 0;
}

int main(void)
{
    init_bdd();
    welcome();
}
