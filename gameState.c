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
#include "Weapon.h"
#include "Armor.h"

int insertPlayer(sqlite3 *db, st_player* p_player, st_level* p_level, int saveId)
{


    // PLAYER ------------------


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

    // WEAPON ------------------

    int idArme = 0;

    Weapon * currentWeapon = (Weapon*) p_player->weapons;
    while (currentWeapon != NULL)
    {

        // Insertion de l'arme dans WEAPON
        char weapon_insert_sql[256];
        snprintf(weapon_insert_sql, sizeof(weapon_insert_sql),
                 "INSERT INTO WEAPON (name, nb_attack_per_round, minDamage, maxDamage, id_sauvegarde) VALUES ('%s', %d, %d, %d, %d);",
                 currentWeapon->name, currentWeapon->nb_attack_per_round,
                 currentWeapon->minDamage, currentWeapon->maxDamage, saveId);

        rc = sqlite3_exec(db, weapon_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données de l'arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        // Récupération de l'ID de l'arme insérée
        int weaponIdFirst = sqlite3_last_insert_rowid(db);

        // Insertion dans PLAYER_WEAPON
        char player_weapon_insert_sql[256];
        snprintf(player_weapon_insert_sql, sizeof(player_weapon_insert_sql),
                 "INSERT INTO PLAYER_WEAPON (player_id, weapon_id, isEquiped, next_weapon_id) VALUES (%d, %d, %d, NULL);",
                 playerId, weaponIdFirst, currentWeapon->isEquipped);

        rc = sqlite3_exec(db, player_weapon_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion de l'association joueur-arme : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        if(idArme != 0)
        {
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

        }

        idArme = weaponIdFirst;
        currentWeapon = (Weapon*) currentWeapon->next;
    }


    // ---- ARMOR -------------


    int idArmor = 0;

    Armor * currentArmor = (Armor*)p_player->armors;

    while (currentArmor != NULL)
    {
        char armor_insert_sql[256];
        snprintf(armor_insert_sql, sizeof(armor_insert_sql),
            "INSERT INTO ARMOR (name, defense, id_sauvegarde) VALUES ('%s', %d, %d);",
            currentArmor->name, currentArmor->defense, saveId);

        rc = sqlite3_exec(db, armor_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données de l'armure : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        // Récupération de l'ID de l'arme insérée
        int armorIdFirst = sqlite3_last_insert_rowid(db);
        // Insertion dans PLAYER_WEAPON
        char player_armor_insert_sql[256];
        snprintf(player_armor_insert_sql, sizeof(player_armor_insert_sql),
            "INSERT INTO PLAYER_ARMOR (player_id, armor_id, isEquiped, next_armor_id) VALUES (%d, %d, %d, NULL);",
            playerId, armorIdFirst, currentArmor->isEquipped);

        rc = sqlite3_exec(db, player_armor_insert_sql, 0, 0, &error_message);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de l'insertion de l'association joueur-armor : %s\n", error_message);
            sqlite3_free(error_message);
            return rc;
        }

        if(idArme != 0)
        {
            char update_prev_weapon_sql[256];
            snprintf(update_prev_weapon_sql, sizeof(update_prev_weapon_sql),
            "UPDATE PLAYER_ARMOR SET next_armor_id = %d WHERE armor_id = %d;",
            armorIdFirst, idArme);

            rc = sqlite3_exec(db, update_prev_weapon_sql, 0, 0, &error_message);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la mise à jour de next_armor_id : %s\n", error_message);
                sqlite3_free(error_message);
                return rc;
            }

        }
        idArmor = armorIdFirst;
        currentArmor = (Armor *)currentArmor->next;
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
}



int insert_monsters(sqlite3 *db, st_level* p_level, int saveId)
{
    char *error_message = 0;
    int rc;

    for (int i = 1; i < MAX_LVL; i++)
    {
        st_level *p_lvl = &p_level[i];


        st_monsters *p_monster = p_lvl->p_monster;

        while (p_monster != NULL)
        { // tant que y'a des monstres :


            const char *insert_monster_sql = "INSERT INTO MONSTRE (number, maxLife, currentLife, attack, p_next, id_sauvegarde) "
                                             "VALUES (?, ?, ?, ?, ?, ?);";


            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, insert_monster_sql, -1, &stmt, 0);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la préparation de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                return rc;
            }

            if (p_monster->p_next)
            {
                p_monster->p_next;
            }
            else
            {
                p_monster->p_next = NULL;
            }

            // Liez les valeurs aux paramètres de la requête
            sqlite3_bind_int(stmt, 1, p_monster->number);
            sqlite3_bind_int(stmt, 2, p_monster->maxLife);
            sqlite3_bind_int(stmt, 3, p_monster->currentLife);
            sqlite3_bind_int(stmt, 4, p_monster->attack);

            if (p_monster->p_next)
            {
                sqlite3_bind_int(stmt, 5, *p_monster->p_next);
            }
            else
            {
                sqlite3_bind_null(stmt, 5); // Aucun monstre suivant
            }
            sqlite3_bind_int(stmt, 6, saveId);

            // Exécutez la requête
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE)
            {
                fprintf(stderr, "Erreur lors de l'exécution de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                return rc;
            }

            // Finalisez la requête
            sqlite3_finalize(stmt);

            // LVL

            // Préparez la requête SQL d'insertion
            const char *insert_level_sql = "INSERT INTO LEVEL (lvl_number, nb_monster, p_monster, id_sauvegarde) VALUES (?, ?, ?, ?);";

            sqlite3_stmt *stmt2;
            rc = sqlite3_prepare_v2(db, insert_level_sql, -1, &stmt2, 0);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erreur lors de la préparation de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                return rc;
            }

            // Liez les valeurs aux paramètres de la requête
            sqlite3_bind_int(stmt2, 1, p_lvl->lvl_number);
            sqlite3_bind_int(stmt2, 2, p_lvl->nb_monster);
            sqlite3_bind_int(stmt2, 3, p_monster->number);
            sqlite3_bind_int(stmt2, 4, saveId);

            // Exécutez la requête
            rc = sqlite3_step(stmt2);
            if (rc != SQLITE_DONE)
            {
                fprintf(stderr, "Erreur lors de l'exécution de la requête d'insertion : %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt2);
                return rc;
            }

            // Finalisez la requête
            sqlite3_finalize(stmt2);

            p_monster = (st_monsters *) p_monster->p_next; // i++
        }
    }
    return SQLITE_OK;
}

int insertTheSAVE(sqlite3 *db, st_player* p_player, st_level* p_level)
{
    // ERROR MSG
    char *error_message = 0;
    // RETURN CODE
    int rc = 0;

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

    // On fait notre requête d'insert
    char sauvegarde_insert_sql[256];
    snprintf(sauvegarde_insert_sql, sizeof(sauvegarde_insert_sql),
             "INSERT INTO SAUVEGARDE (currentLevel, date) VALUES (%d, '%s');",
             p_level->lvl_number, datetime);

    // on l'éxecute
    rc = sqlite3_exec(db, sauvegarde_insert_sql, 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'insertion des données de sauvegarde : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }

    // On stock l'id de la sauvegarde, c'est lui qui va nous permettre de lier toutes les tables, quand on va load la save.

    // Validez la transaction
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &error_message);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la validation de la transaction : %s\n", error_message);
        sqlite3_free(error_message);
        return rc;
    }


    // RECUP DU SAVE ID
    sqlite3_int64 sauvegarde_id = sqlite3_last_insert_rowid(db);
    int saveId = (int)sauvegarde_id;


    // LES FONCTIONS d'insertions avec en paramètre l'id de la sauvegarde.

    insertPlayer(db, p_player, p_level, saveId);

    insert_monsters(db, p_level, saveId);

    printf("Insertion en base de donnés réussis.\n");

    return SQLITE_OK; // Tout s'est bien passé
}
