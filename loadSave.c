//
// Created by ethan on 05/11/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "sqlite3.h"
#include "monster.h"
#include "player.h"
#include "level.h"

int displayLatestSaves(void)
{
    // ouverture BDD
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // ON SELECTE LES 5 DERNIRERE SAUVEGARDE EN DATE

    const char *select_latest_saves_sql = "SELECT * FROM SAUVEGARDE ORDER BY date ASC LIMIT 5;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, select_latest_saves_sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return rc;
    }


    int i = 1;
    int choixSave = 0;
    printf("Quelle sauvegardes voulez-vous charger ?\n");
    printf("0 - Nouvelle partie \n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int sauvegarde_id = sqlite3_column_int(stmt, 0);
        int currentLevel = sqlite3_column_int(stmt, 1);
        const char *date = (const char *)sqlite3_column_text(stmt, 2);

        printf("%d - ID: %d, Niveau actuel: %d, Date: %s\n", sauvegarde_id, sauvegarde_id, currentLevel, date);
        i++;
    }
    scanf("%d", &choixSave);

    if (i == 1)
    {
        return printf("Aucune sauvegarde n'existe.\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return choixSave;
}






