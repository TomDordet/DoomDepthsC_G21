#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sqlite3.h"
#include "Player.h"
#include "Weapon.h"

// fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
st_player * create_player (int id_db)
{
    printf("DEBUG --- %s() ----id_db %d \n", __FUNCTION__ ,id_db);
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.
    printf("DEBUG :: alloc p_player [%d] = %p \n",1,p_player); // printf pour débug, vérifier que le joueur est bien créer/allouer.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.

    if (id_db == 0)
    {
        //default value, Hardcoded value.
        p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
        p_player->currentLife = p_player->maxLife;
        p_player->attack = 10;
        p_player->defense = 10;
        p_player->weapons = NULL;
        p_player->armors = NULL;

        // créer les armes de base :

        Weapon *startWeapon = createWeapon(1, 2, 5);
        startWeapon->isEquipped = 1;
        startWeapon->name = "Baton en bois";
        printf("%s\n",startWeapon->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *startWeapon);

        Weapon *weapon2 = createWeapon(1, 2, 5);
        printf("%s\n",weapon2->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon2);

        Weapon *weapon3 = createWeapon(3, 7, 5);
        printf("%s\n",weapon3->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon3);
        Weapon *weapon4 = createWeapon(2, 20, 22);
        printf("%s\n",weapon4->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon4);

        /*
        //PARTIE TEST WEAPONS
        Weapon *weapon2 = createWeapon(1, 2, 5);
        printf("%s\n",weapon2->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon2);
        Weapon *weapon3 = createWeapon(3, 7, 5);
        printf("%s\n",weapon3->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon3);
        Weapon *weapon4 = createWeapon(2, 20, 22);
        printf("%s\n",weapon4->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon4);
        Weapon *weapon5 = createWeapon(3, 7, 5);
        printf("%s\n",weapon5->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon5);

        printf("%d\n", countWeaponsPlayer(p_player->weapons));

        WeaponsPlayer *weaponsPlayer = getWeaponNumberToWeaponsPlayer(p_player->weapons, 4);
        printf("Nom : %s\n", weaponsPlayer->weapon.name);

        Weapon *weapon6 = createWeapon(2, 3, 14);
        printf("%s\n",weapon6->name);
        p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon6);

        swapWeaponsPlayer(p_player->weapons, *weapon6);
        */

        //FIN PARTIE TEST ARMORS


        Armor *startArmor = createArmor(1);
        startArmor->isEquipped = 1;
        startArmor->name = "Tenue simple";
        printf("%s\n",startArmor->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *startArmor);

        Armor *armor2 = createArmor(2);
        printf("%s\n",armor2->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor2);


        /*
        //PARTIE TEST WEAPONS
        Armor *armor2 = createArmor(2);
        printf("%s\n",armor2->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor2);
        Armor *armor3 = createArmor(3);
        printf("%s\n",armor3->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor3);
        Armor *armor4 = createArmor(4);
        printf("%s\n",armor4->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor4);
        Armor *armor5 = createArmor(5);
        printf("%s\n",armor5->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor5);

        printf("%d\n", countArmorsPlayer(p_player->armors));

        ArmorsPlayer *armorsPlayer = getArmorNumberToArmorsPlayer(p_player->armors, 4);
        printf("Nom : %s\n", armorsPlayer->armor.name);

        Armor *armor6 = createArmor(6);
        printf("%s\n",armor6->name);
        p_player->armors = addArmorsPlayer(p_player->armors, *armor6);

        swapArmorsPlayer(p_player->armors, *armor6);
        */
        //FIN PARTIE TEST ARMORS
    }
    else
    {
        // PLAYER
        int rc = 0;
        sqlite3 *db;
        rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
        if (rc)
        {
            fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        }


        const char *select_player_sql = "SELECT * FROM JOUEUR WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, select_player_sql, -1, &stmt, 0);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        printf("RQT PLAYER OK\n");


        sqlite3_bind_int(stmt, 1, id_db);

        printf("ID DB : %d\n", id_db);

        rc = sqlite3_step(stmt);

        int playerId = sqlite3_column_int(stmt, 0);
        int playerMaxLife = sqlite3_column_int(stmt, 1);
        int playerCurrentLife = sqlite3_column_int(stmt, 2);
        int playerAttack = sqlite3_column_int(stmt, 3);


        p_player->maxLife = playerMaxLife;
        p_player->currentLife = playerCurrentLife;
        p_player->attack = playerAttack;

        sqlite3_finalize(stmt);

        printf("PLAYER FINIS\n");


        // WEAPON


        rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
        if (rc)
        {
            fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        }

        // LA REQUETE POUR REQUETE DES INFOS SUR LE NIVEAU
        const char *select_minMaxWeapon_sql = "SELECT COUNT(*) FROM WEAPON WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt2;
        rc = sqlite3_prepare_v2(db, select_minMaxWeapon_sql, -1, &stmt2, 0);

        printf("RQT INFOS WEAPONS ID OK\n");
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        // BIND LES ZONES INCONNUES AVEC DES VARIABLES
        sqlite3_bind_int(stmt2, 1, id_db);


        rc = sqlite3_step(stmt2);

        int countWeapon = sqlite3_column_int(stmt2, 0);

        printf("NB WEAPON : %d\n", countWeapon);

        // NOUVELLE REQUETE POUR RECUP LES DATA
        const char *select_levels_sql = "SELECT W.nb_attack_per_round, W.minDamage, W.maxDamage, W.name, P.isEquiped "
                                        "FROM WEAPON W "
                                        "INNER JOIN PLAYER_WEAPON P ON W.id = P.weapon_id "
                                        "WHERE P.player_id = ?;";
        sqlite3_stmt *stmt3;
        rc = sqlite3_prepare_v2(db, select_levels_sql, -1, &stmt3, 0);


        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt3, 1, playerId);

        printf("RQT INFO WEAPON OK\n");
        // recup data :

        int i = 0;

        while ((rc = sqlite3_step(stmt3)) == SQLITE_ROW && i < countWeapon)
        {

            int nbAttck = sqlite3_column_int(stmt3, 0);
            int minDamage = sqlite3_column_int(stmt3, 1);
            int maxDamage = sqlite3_column_int(stmt3, 2);
            const char *name = (const char *)sqlite3_column_text(stmt3, 3);
            int isEquiped = sqlite3_column_int(stmt3, 4);

            printf("NB ATTACK : %d\n", nbAttck);
            printf("MAX DAMAGE : %d\n", maxDamage);
            printf("MIN DAMAGE : %d\n", minDamage);
            printf("NAME : %s\n", name);
            printf("EQUIPER : %d\n", isEquiped);

            Weapon *newWeapon = createWeapon(nbAttck, minDamage, maxDamage);

            printf("CREATE WEAPON OK\n");

            newWeapon->isEquipped = isEquiped;

            printf("EQUIPED OK\n");

            p_player->weapons = addWeaponsPlayer(p_player->weapons, *newWeapon);

            printf("ADD WEAPON PLAYER OK\n");

            newWeapon->name = strdup(name);

            printf("DUMP NAME OK\n");

            i++; // Incrémentez le compteur
        }

        printf("FINITO\n\n");
        sqlite3_finalize(stmt2);
        sqlite3_finalize(stmt3);

        sqlite3_close(db);

    }
    return p_player;
}


st_player * delete_player (st_player * p_player) //Supprime le joueur.
{
    //débug de vérification du free de l'ensemble des armes du joueur
    //je le fais ici car les armes ont besoin d'être free seulement lorsque le joueur meurt.
    deleteWeaponsPlayer(p_player->weapons);
    deleteArmorsPlayer(p_player->armors);

    printf("DEBUG :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return NULL;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque :  : %d \n", p_player->attack );

    //Affiche l'arme équipée
    WeaponsPlayer *tmp = p_player->weapons;
    while (tmp != NULL)
    {
        if (tmp->weapon.isEquipped == 1) {
            printf("Arme equipee : %s\n", tmp->weapon.name);
            break;
        }
        tmp = tmp->next;
    }
    //Affiche l'armure équipée
    ArmorsPlayer *tmp2 = p_player->armors;
    while (tmp2 != NULL) {
        if (tmp2->armor.isEquipped == 1) {
            printf("Armure equipee : %s\n", tmp2->armor.name);
            break;
        }
        tmp2 = tmp2->next;
    }
    return 0;
}