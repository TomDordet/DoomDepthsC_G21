#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Player.h"

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
        p_player->weapons = 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.
    }
    else
    {
        //get db (id_db)
        p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
        p_player->currentLife = p_player->maxLife;
        p_player->attack = 10;
        p_player->defense = 10;
        p_player->weapons = 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.

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