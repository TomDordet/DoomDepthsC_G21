#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"
#include "sqlite3.h"

int insertTheSAVE(sqlite3 *db, st_player* p_player, st_level* p_level);

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