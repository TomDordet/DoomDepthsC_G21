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
    p_player->weapons = malloc(sizeof(Weapon) * 5);
    //partie test
    p_player->weapons[0].name = "Epee robuste";
    p_player->weapons[0].nb_attack_per_round = 1;
    p_player->weapons[0].minDamage = 15;
    p_player->weapons[0].maxDamage = 22;
    p_player->weapons[0].isEquipped = 0;
    p_player->weapons[0].canBeDisplayed = 1;

    p_player->weapons[1].name = "Epee d'entrainement";
    p_player->weapons[1].nb_attack_per_round = 4;
    p_player->weapons[1].minDamage = 12;
    p_player->weapons[1].maxDamage = 18;
    p_player->weapons[1].isEquipped = 0;
    p_player->weapons[1].canBeDisplayed = 0;

    p_player->weapons[2].name = "Sabre de Namec";
    p_player->weapons[2].nb_attack_per_round = 2;
    p_player->weapons[2].minDamage = 27;
    p_player->weapons[2].maxDamage = 34;
    p_player->weapons[2].isEquipped = 1;
    p_player->weapons[2].canBeDisplayed = 1;

    p_player->weapons[3].name = "Baton de bois";
    p_player->weapons[3].nb_attack_per_round = 1;
    p_player->weapons[3].minDamage = 1;
    p_player->weapons[3].maxDamage = 8;
    p_player->weapons[3].isEquipped = 0;
    p_player->weapons[3].canBeDisplayed = 0;

    p_player->weapons[4].name = "Epee maudite";
    p_player->weapons[4].nb_attack_per_round = 1;
    p_player->weapons[4].minDamage = 99;
    p_player->weapons[4].maxDamage = 109;
    p_player->weapons[4].isEquipped = 0;
    p_player->weapons[4].canBeDisplayed = 1;

    return p_player;
}


int delete_player (st_player* p_player) //Supprime le joueur.
{
    //débug de vérification du free de l'ensemble des armes du joueur
    //je le fais ici car les armes ont besoin d'être free seulement lorsque le joueur meurt.
    printf("debug :: free p_player->weapons = %p\n", p_player->weapons);
    free(p_player->weapons);

    printf("debug :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return 0;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque : %d \n", p_player->attack );
    for (int i = 0; i < 5; ++i) {
        if(p_player->weapons[i].isEquipped == 1){
            printf("Arme equipee : %s\n", p_player->weapons[i].name);
            break;
        }
    }
    return 0;
}