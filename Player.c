#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"

st_player* create_player () // fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
{
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.
    printf("debug :: alloc p_player [%d] = %p \n",1,p_player); // printf pour débug, vérifier que le joueur est bien créer/allouer.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.
    p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
    p_player->currentLife = p_player->maxLife;
    p_player->nb_attack_per_round = rand() % (10 + 1); // Attaque par round, c'était évoqué dans le sujet, donc je l'ai mis, mais faudras approfondir sur ce point, et se mettre d'accord sur comment ca marche cette historie de d'attaque par round. (pour moi un round = une attaque chacun (MyPlayer, monstre(s) ??).
    p_player->attack = rand() % (10 + 1);
    p_player->defense = rand() % (10 + 1);
    p_player->weapons = rand() % 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.

    return p_player;
}


int delete_player (st_player * p_player) //Supprime le joueur.
{
    printf("debug :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return 0;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque :  : %d \n", p_player->attack );
    printf("Armes : %d \n", p_player->weapons);
    return 0;
}