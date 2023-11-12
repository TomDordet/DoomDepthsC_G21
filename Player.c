#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sqlite3.h"
#include "Player.h"
#include "Weapon.h"
#include "Armor.h"

// fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
st_player * create_player(int id_db){
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

        Weapon *weapon1 = createWeapon(1, 2, 5);
        weapon1->isEquipped = 1;
        weapon1->name = "Baton en bois";
        printf("%s\n",weapon1->name);
        p_player->weapons = addWeaponsPlayer(p_player, weapon1);

        Weapon *weapon2 = createWeapon(1, 2, 5);
        printf("%s\n",weapon2->name);
        p_player->weapons = addWeaponsPlayer(p_player, weapon2);

        Weapon *weapon3 = createWeapon(3, 7, 5);
        printf("%s\n",weapon3->name);
        p_player->weapons = addWeaponsPlayer(p_player, weapon3);

        Weapon *weapon4 = createWeapon(2, 20, 22);
        printf("%s\n",weapon4->name);
        p_player->weapons = addWeaponsPlayer(p_player, weapon4);

        Weapon *weapon5 = createWeapon(3, 7, 5);
        printf("%s\n",weapon5->name);
        p_player->weapons = addWeaponsPlayer(p_player, weapon5);

        printf("DEBUG:: Nb arm is p_player->weapons %d\n", countWeaponsPlayer(p_player));

        Weapon * weapon_debug = getWeaponNumberToWeaponsPlayer(p_player, 4);
        printf("DEBUG Nom : %s\n", weapon_debug->name);

        //FIN PARTIE TEST ARMORS

        Armor *armor1 = createArmor(1);
        armor1->isEquipped = 1;
        armor1->name = "Tenue simple";
        printf("%s\n",armor1->name);
        p_player->armors = addArmorsPlayer(p_player, armor1);

        //PARTIE TEST WEAPONS
        Armor *armor2 = createArmor(2);
        printf("%s\n",armor2->name);
        p_player->armors = addArmorsPlayer(p_player, armor2);

        Armor *armor3 = createArmor(3);
        printf("%s\n",armor3->name);
        p_player->armors = addArmorsPlayer(p_player, armor3);

        Armor *armor4 = createArmor(4);
        printf("%s\n",armor4->name);
        p_player->armors = addArmorsPlayer(p_player, armor4);

        Armor *armor5 = createArmor(5);
        printf("%s\n",armor5->name);
        p_player->armors = addArmorsPlayer(p_player, armor5);

        printf("DEBUG:: NB Armor in player %d\n", countArmorsPlayer(p_player));

        Armor *armor_debug = getArmorNumberToArmorsPlayer(p_player, 4);
        printf("DEBUG Nom : %s\n", armor_debug->name);
        //FIN PARTIE TEST ARMORS
    }
    else
    {
        //on a une DB
        int rc = 0;
        sqlite3 *db;
        rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
        if (rc)
        {
            fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
            free (p_player);
            return NULL;
        }

        // PLAYER --------
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

        sqlite3_step(stmt);

        int playerId = sqlite3_column_int(stmt, 0);
        int playerMaxLife = sqlite3_column_int(stmt, 1);
        int playerCurrentLife = sqlite3_column_int(stmt, 2);
        int playerAttack = sqlite3_column_int(stmt, 3);


        p_player->maxLife = playerMaxLife;
        p_player->currentLife = playerCurrentLife;
        p_player->attack = playerAttack;
        p_player->weapons = NULL;
        p_player->armors = NULL;

        sqlite3_finalize(stmt);

        printf("PLAYER FINIS\n");



        // WEAPON -----------------


        // LA REQUETE POUR REQUETE DES INFOS SUR LE NIVEAU
        const char *select_minMaxWeapon_sql = "SELECT COUNT(*) FROM WEAPON WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt2;
        rc = sqlite3_prepare_v2(db, select_minMaxWeapon_sql, -1, &stmt2, 0);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        // BIND LES ZONES INCONNUES AVEC DES VARIABLES
        sqlite3_bind_int(stmt2, 1, id_db);

        sqlite3_step(stmt2);

        int countWeapon = sqlite3_column_int(stmt2, 0);

        printf("NB WEAPON : %d\n", countWeapon);

        // NOUVELLE REQUETE POUR RECUP LES DATA
        const char *select_weapons_sql = "SELECT W.nb_attack_per_round, W.minDamage, W.maxDamage, W.name, P.isEquiped "
                                        "FROM WEAPON W "
                                        "INNER JOIN PLAYER_WEAPON P ON W.id = P.weapon_id "
                                        "WHERE P.player_id = ?;";
        sqlite3_stmt *stmt3;
        rc = sqlite3_prepare_v2(db, select_weapons_sql, -1, &stmt3, 0);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt3, 1, playerId);

        sqlite3_step(stmt3);

        int i = 0;

        while ( (i < countWeapon) )
        {

            int nbAttck = sqlite3_column_int(stmt3, 0);
            int minDamage = sqlite3_column_int(stmt3, 1);
            int maxDamage = sqlite3_column_int(stmt3, 2);
            const char *nameWeapon = (const char *)sqlite3_column_text(stmt3, 3);
            int isEquipedWeapon = sqlite3_column_int(stmt3, 4);

            Weapon *newWeapon = createWeaponSave(nbAttck, minDamage, maxDamage, nameWeapon);

            newWeapon->isEquipped = isEquipedWeapon;

            p_player->weapons = addWeaponsPlayer(p_player, newWeapon);

            i++; // Incrémentez le compteur
            sqlite3_step(stmt3);
        }

        printf("FINITO ARME\n\n");
        sqlite3_finalize(stmt2);
        sqlite3_finalize(stmt3);


        // ARMOR -----------------

        // LA REQUETE POUR REQUETE DES INFOS SUR LE NIVEAU
        const char *select_minMaxArmor_sql = "SELECT COUNT(*) FROM ARMOR WHERE id_sauvegarde = ?;";
        sqlite3_stmt *stmt4;
        rc = sqlite3_prepare_v2(db, select_minMaxArmor_sql, -1, &stmt4, 0);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        // BIND LES ZONES INCONNUES AVEC DES VARIABLES
        sqlite3_bind_int(stmt4, 1, id_db);

        sqlite3_step(stmt4);

        int countArmor = sqlite3_column_int(stmt4, 0);

        printf("NB ARMOR : %d\n", countArmor);

        // NOUVELLE REQUETE POUR RECUP LES DATA
        const char *select_armors_sql = "SELECT A.name, A.defense, P.isEquiped "
                                "FROM ARMOR A "
                                "INNER JOIN PLAYER_ARMOR P ON A.id = P.armor_id "
                                "WHERE P.player_id = ?;";
        sqlite3_stmt *stmt5;
        rc = sqlite3_prepare_v2(db, select_armors_sql, -1, &stmt5, 0);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt5, 1, playerId);

        printf("RQT INFO ARMOR OK\n");
        // recup data :

        sqlite3_step(stmt5);

        int j = 0;

        while ((j < countArmor))
        {

            const char *nameArmor = (const char *)sqlite3_column_text(stmt5, 0);
            int defense = sqlite3_column_int(stmt5, 1);
            int isEquipedArmor = sqlite3_column_int(stmt5, 2);
            printf("------------------------- (%d)\n", j);
            printf("ARMOR NAME : %s\n\n", nameArmor);

            Armor *newArmor = createArmorSave(defense, nameArmor);

            newArmor->isEquipped = isEquipedArmor;

            p_player->armors = addArmorsPlayer(p_player, newArmor);

            j++; // Incrémentez le compteur
            sqlite3_step(stmt5);
        }

        printf("FINITO ARMOR\n\n");
        sqlite3_finalize(stmt4);
        sqlite3_finalize(stmt5);

        sqlite3_close(db);

    }
    return p_player;
}


st_player * delete_player (st_player * p_player) //Supprime le joueur.
{
    //débug de vérification du free de l'ensemble des armes du joueur
    //je le fais ici car les armes ont besoin d'être free seulement lorsque le joueur meurt.
    printf("DEBUG %s() Line %d \n ",__FUNCTION__ ,__LINE__);
    deleteWeaponsPlayer(p_player);
     printf("DEBUG %s() Line %d \n ",__FUNCTION__ ,__LINE__);
    //deleteArmorsPlayer(p_player->armors);
    printf("DEBUG %s() Line %d \n ",__FUNCTION__ ,__LINE__);
    printf("DEBUG :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    printf("DEBUG %s() Line %d \n ",__FUNCTION__ ,__LINE__);
    return NULL;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque :  : %d \n", p_player->attack );

    //Affiche l'arme équipée
    Weapon * tmp = (Weapon *)p_player->weapons;
    while (tmp != NULL)
    {
        if (tmp->isEquipped == 1) {
            printf("Arme equipee : %s\n", tmp->name);
            break;
        }
        tmp = (Weapon *)tmp->next;
    }

    /*
    //Affiche l'armure équipée
    ArmorsPlayer *tmp2 = p_player->armors;
    while (tmp2 != NULL) {
        if (tmp2->armor.isEquipped == 1) {
            printf("Armure equipee : %s\n", tmp2->armor.name);
            break;
        }
        tmp2 = tmp2->next;
    }*/
    return 0;
}