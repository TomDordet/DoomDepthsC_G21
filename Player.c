#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "sqlite3.h"

// fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
st_player * create_player (int save_id)
{
    printf("DEBUG --- %s() ----id_db %d \n", __FUNCTION__ ,save_id);
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.
    printf("DEBUG :: alloc p_player [%d] = %p \n",1,p_player); // printf pour débug, vérifier que le joueur est bien créer/allouer.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.

    if (save_id == 0)
    {
        //default value, Hardcoded value.
        p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
        p_player->currentLife = p_player->maxLife;
        p_player->attack = 10;
        p_player->defense = 10;
        p_player->weapons = 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.
    }
    else
    {
        int rc = 0;
        sqlite3 *db;
        rc = sqlite3_open("D:/Bilal/Code/DoomDepths/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
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

        sqlite3_bind_int(stmt, 1, save_id);

        rc = sqlite3_step(stmt);

        int playerMaxLife = sqlite3_column_int(stmt, 1);
        int playerCurrentLife = sqlite3_column_int(stmt, 2);
        int playerAttack = sqlite3_column_int(stmt, 3);


        p_player->maxLife = playerMaxLife;
        p_player->currentLife = playerCurrentLife;
        p_player->attack = playerAttack;


        sqlite3_finalize(stmt);
        sqlite3_close(db);


    }
    return p_player;
}


st_player * delete_player (st_player * p_player) //Supprime le joueur.
{
    printf("DEBUG :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return NULL;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque :  : %d \n", p_player->attack );
    printf("Armes : %d \n", p_player->weapons);
    return 0;
}