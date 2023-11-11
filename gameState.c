//
// Created by ethan on 01/11/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "sqlite3.h"
#include "monster.h"
#include "player.h"
#include "level.h"

/*
int insertPlayer(sqlite3 *db, st_player* p_player, st_level* p_level, int saveId, WeaponsPlayer* weapons)
{
    char *error_message = 0;
    int rc = 0;
    // char *datetime = getDate();

    // Commencez une transaction pour assurer la cohérence des données
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors du début de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }

    // Utilisez cet ID pour insérer les données du joueur dans la table JOUEUR
    char joueur_insert_sql[256];
    snprintf(joueur_insert_sql, sizeof(joueur_insert_sql),
             "INSERT INTO JOUEUR (maxLife, currentLife, attack, id_niveau, id_sauvegarde) VALUES (%d, %d, %d, %d, %d);",
             p_player->maxLife, p_player->currentLife, p_player->attack, p_level->lvl_number, saveId);

    rc = sqlite3_exec(db, joueur_insert_sql, 0, 0, &error_message);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'insertion des données du joueur : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }

    int playerId = sqlite3_last_insert_rowid(db);


    WeaponsPlayer* currentWeapon = weapons;
    WeaponsPlayer* nextWeapon = currentWeapon->next;

    while (currentWeapon != NULL)
    {
        char weapon_insert_sql[256];
        snprintf(weapon_insert_sql, sizeof(weapon_insert_sql),
                 "INSERT INTO WEAPON (name, nb_attack_per_round, minDamage, maxDamage, id_sauvegarde) VALUES ('%s', %d, %d, %d, %d);",
                 currentWeapon->weapon.name, currentWeapon->weapon.nb_attack_per_round,
                 currentWeapon->weapon.minDamage, currentWeapon->weapon.maxDamage, saveId);

        rc = sqlite3_exec(db, weapon_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données de l'arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        // Récupérez l'ID de l'arme insérée
        int weaponId = sqlite3_last_insert_rowid(db);

        if (currentWeapon->next) {
            // Insérer l'arme suivante
            char next_weapon_insert_sql[256];
            snprintf(next_weapon_insert_sql, sizeof(next_weapon_insert_sql),
                     "INSERT INTO WEAPON (name, nb_attack_per_round, minDamage, maxDamage, id_sauvegarde) VALUES ('%s', %d, %d, %d, %d);",
                     currentWeapon->next->weapon.name, currentWeapon->next->weapon.nb_attack_per_round,
                     currentWeapon->next->weapon.minDamage, currentWeapon->next->weapon.maxDamage, saveId);

            rc = sqlite3_exec(db, next_weapon_insert_sql, 0, 0, &error_message);
            if (rc != SQLITE_OK) {
                // Gérer l'erreur...
            }

            // Récupérer l'ID de l'arme suivante
            int nextWeaponId = sqlite3_last_insert_rowid(db);

            // Mettre à jour l'entrée de l'arme actuelle avec l'ID de l'arme suivante
            char update_current_weapon_sql[256];
            snprintf(update_current_weapon_sql, sizeof(update_current_weapon_sql),
                     "UPDATE WEAPON SET next_weapon_id = %d WHERE id = %d;", nextWeaponId, weaponId);

            rc = sqlite3_exec(db, update_current_weapon_sql, 0, 0, &error_message);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la mise à jour de l'arme actuelle : %s\n", error_message);
                sqlite3_free(error_message);
                return rc;
            }
        }

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion de l'association joueur-arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        currentWeapon = currentWeapon->next;
        if (nextWeapon != NULL)
            nextWeapon = nextWeapon->next;
    }


    // Validez la transaction
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la validation de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }
    return SQLITE_OK; // Tout s'est bien passé
}*/


int insertPlayer(sqlite3 *db, st_player* p_player, st_level* p_level, int saveId, WeaponsPlayer* weapons)
{
    char *error_message = 0;
    int rc = 0;

    // Commencez une transaction pour assurer la cohérence des données
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors du début de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }
    printf("DEBUG :: BEGIN TRANSAC\n");

    // Utilisez cet ID pour insérer les données du joueur dans la table JOUEUR
    char joueur_insert_sql[256];
    snprintf(joueur_insert_sql, sizeof(joueur_insert_sql),
             "INSERT INTO JOUEUR (maxLife, currentLife, attack, id_niveau, id_sauvegarde) VALUES (%d, %d, %d, %d, %d);",
             p_player->maxLife, p_player->currentLife, p_player->attack, p_level->lvl_number, saveId);

    rc = sqlite3_exec(db, joueur_insert_sql, 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'insertion des données du joueur : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }

    printf("JOUEUR INSERER\n");

    int playerId = sqlite3_last_insert_rowid(db);
    printf("ID JOUEUR : %d\n\n", playerId);
    //int prevWeaponId = -1; // Variable pour stocker l'ID de l'arme précédente


    int idArme = 0;

    WeaponsPlayer* currentWeapon = weapons;

    while (currentWeapon != NULL)
    {
        printf(" ARME NAME : %s\n", currentWeapon->weapon.name);
        // Insertion de l'arme dans WEAPON
        char weapon_insert_sql[256];
        snprintf(weapon_insert_sql, sizeof(weapon_insert_sql),
                 "INSERT INTO WEAPON (name, nb_attack_per_round, minDamage, maxDamage, id_sauvegarde) VALUES ('%s', %d, %d, %d, %d);",
                 currentWeapon->weapon.name, currentWeapon->weapon.nb_attack_per_round,
                 currentWeapon->weapon.minDamage, currentWeapon->weapon.maxDamage, saveId);

        rc = sqlite3_exec(db, weapon_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données de l'arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }
        printf("INSERTION ARME : %s\n", currentWeapon->weapon.name);

        // Récupération de l'ID de l'arme insérée
        int weaponIdFirst = sqlite3_last_insert_rowid(db);
        printf("ID DE L'ARME : %d\n", weaponIdFirst);

        printf("INSERT PLAYER-WEAPON :\n");
        printf("PLAYER ID : %d\n", playerId);
        printf("ID ARME INSERER : %d\n", weaponIdFirst);
        printf(" IS EQUIPPED : %d\n", currentWeapon->weapon.isEquipped);

        // Insertion dans PLAYER_WEAPON
        char player_weapon_insert_sql[256];
        snprintf(player_weapon_insert_sql, sizeof(player_weapon_insert_sql),
                 "INSERT INTO PLAYER_WEAPON (player_id, weapon_id, isEquiped, next_weapon_id) VALUES (%d, %d, %d, NULL);",
                 playerId, weaponIdFirst, currentWeapon->weapon.isEquipped);

        rc = sqlite3_exec(db, player_weapon_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion de l'association joueur-arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }
        printf("PLAYER-WEAPON OK\n\n");

        if(idArme != 0)
        {
            printf("UPDATE CONDITION\n");
            printf("ID ARME INSERE: %d\n", idArme);
            printf("ID ARME PREV : %d\n", weaponIdFirst);
            char update_prev_weapon_sql[256];
            snprintf(update_prev_weapon_sql, sizeof(update_prev_weapon_sql),
                     "UPDATE PLAYER_WEAPON SET next_weapon_id = %d WHERE weapon_id = %d;",
                     weaponIdFirst, idArme);

            rc = sqlite3_exec(db, update_prev_weapon_sql, 0, 0, &error_message);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la mise à jour de next_weapon_id : %s\n", error_message);
                sqlite3_free(error_message);
                return rc;
            }
            printf("UPDATE PLAYER-WEAPON OK\n\n");

        }

        idArme = weaponIdFirst;
        printf("ID ARME NEW : %d\n", idArme);
        currentWeapon = currentWeapon->next;
        printf("ARME SUIVANTE\n\n");
    }


    // Validez la transaction
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la validation de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }
    printf("DEBUG :: fin (commit ok) \n\n");

    return SQLITE_OK; // Tout s'est bien passé
}



int insert_monsters(sqlite3 *db, st_level* p_level, int saveId)
{
    char *error_message = 0;
    int rc;

    for (int i = 1; i < MAX_LVL; i++)
    {
        st_level *p_lvl = &p_level[i];

        printf("Niveau %d\n", p_lvl->lvl_number);
        printf("Nombre de monstres : %d\n", p_lvl->nb_monster);

        st_monsters *p_monster = p_lvl->p_monster;

        while (p_monster != NULL)
        { // tant que y'a des monstres :

            printf("préparation de la RQT\n");

            const char *insert_monster_sql = "INSERT INTO MONSTRE (number, maxLife, currentLife, attack, p_next, id_sauvegarde) "
                                             "VALUES (?, ?, ?, ?, ?, ?);";


            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, insert_monster_sql, -1, &stmt, 0);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la préparation de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                return rc;
            }
            printf("préparation RQT ok \n");

            printf("--- NIVEAU %d, MONSTRE %d -----\n", p_level[i].lvl_number, p_monster->number);
            printf("Monster number: %d\n", p_monster->number);
            printf("Max Life: %d\n", p_monster->maxLife);
            printf("Current Life: %d\n", p_monster->currentLife);
            printf("Attack: %d\n", p_monster->attack);

            if (p_monster->p_next)
            {
                printf("Next monster: %p\n", p_monster->p_next);
            }
            else
            {
                p_monster->p_next = NULL;
                printf("No next monster %p\n", p_monster->p_next);
            }
            printf("SAVE ID : %d\n", saveId);

            printf("p_next ok\n");

            // Liez les valeurs aux paramètres de la requête
            sqlite3_bind_int(stmt, 1, p_monster->number);
            sqlite3_bind_int(stmt, 2, p_monster->maxLife);
            sqlite3_bind_int(stmt, 3, p_monster->currentLife);
            sqlite3_bind_int(stmt, 4, p_monster->attack);

            printf("premier bind value = ok\n");

            if (p_monster->p_next)
            {
                sqlite3_bind_int(stmt, 5, *p_monster->p_next);
            }
            else
            {
                sqlite3_bind_null(stmt, 5); // Aucun monstre suivant
            }
            sqlite3_bind_int(stmt, 6, saveId);

            printf("p_next bind ok\n");


            // Exécutez la requête
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE)
            {
                fprintf(stderr, "Erreur lors de l'exécution de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                return rc;
            }

            printf("execution faite\n");
            // Finalisez la requête
            sqlite3_finalize(stmt);
            printf("MONSTRE FINI\n");


            // LVL
            printf("Debut insert niveau \n");
            // Récupérez l'ID du monstre que vous venez d'insérer.

            //sqlite3_int64 monster_id = sqlite3_last_insert_rowid(db);
            //int mstrId = (int)monster_id;


            // Préparez la requête SQL d'insertion
            const char *insert_level_sql = "INSERT INTO LEVEL (lvl_number, nb_monster, p_monster, id_sauvegarde) VALUES (?, ?, ?, ?);";

            printf("Creer RQT \n");
            sqlite3_stmt *stmt2;
            rc = sqlite3_prepare_v2(db, insert_level_sql, -1, &stmt2, 0);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la préparation de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                return rc;
            }

            printf("prepare RQT OK\n");


            printf("LEVEL NUMBER %d \n", p_lvl->lvl_number);
            printf("nb monster  %d \n", p_lvl->nb_monster);
            printf("NUM DU MONSTRE WALLAH %lld \n", p_monster->number);
            printf("SAVE ID : %d\n", saveId);
            // Liez les valeurs aux paramètres de la requête
            sqlite3_bind_int(stmt2, 1, p_lvl->lvl_number);
            sqlite3_bind_int(stmt2, 2, p_lvl->nb_monster);
            sqlite3_bind_int(stmt2, 3, p_monster->number);
            sqlite3_bind_int(stmt2, 4, saveId);

            printf("BIND VALUE OK  \n");

            // Exécutez la requête
            rc = sqlite3_step(stmt2);
            if (rc != SQLITE_DONE)
            {
                fprintf(stderr, "Erreur lors de l'exécution de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt2);
                return rc;
            }

            printf("EXEC RQT \n");
            // Finalisez la requête
            sqlite3_finalize(stmt2);

            printf("LVL FINI  \n");
            p_monster = (st_monsters *) p_monster->p_next; // i++
        }
    }
    return SQLITE_OK;
}

int insertTheSAVE(sqlite3 *db, st_player* p_player, st_level* p_level, WeaponsPlayer* weapons)
{
    // ERROR MSG
    char *error_message = 0;
    // RETURN CODE
    int rc = 0;

    printf("DEBUT INSERTION \n");
    // GET DATE (pour la date de la sauvegarde)
    char datetime[20];
    time_t now = time(0);
    strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));


    // on fait une transaction, pour assurer la cohérence des données (ceux qui ont fait du PHP, vous captez)
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors du début de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }
    printf("TRANSACTION OK \n");
    // On fait notre requête d'insert
    char sauvegarde_insert_sql[256];
    snprintf(sauvegarde_insert_sql, sizeof(sauvegarde_insert_sql),
             "INSERT INTO SAUVEGARDE (currentLevel, date) VALUES (%d, '%s');",
             p_level->lvl_number, datetime);

    printf("RQT OK \n");
    // on l'éxecute
    rc = sqlite3_exec(db, sauvegarde_insert_sql, 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'insertion des données de sauvegarde : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }

    printf("RQT EXEC \n");
    // On stock l'id de la sauvegarde, c'est lui qui va nous permettre de lier toutes les tables, quand on va load la save.

    // Validez la transaction
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la validation de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }
    printf("TRANSACTION OK\n");


    // RECUP DU SAVE ID
    sqlite3_int64 sauvegarde_id = sqlite3_last_insert_rowid(db);
    int saveId = (int)sauvegarde_id;


    // LES FONCTIONS d'insertions avec en paramètre l'id de la sauvegarde.

    insertPlayer(db, p_player, p_level, saveId, weapons);
    printf("PLAYER OK \n");

    insert_monsters(db, p_level, saveId);


    return SQLITE_OK; // Tout s'est bien passé
}
