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
    p_player->weapons = NULL;

    Weapon *startWeapon = createWeapon(1, 2, 5);
    startWeapon->isEquipped = 1;
    startWeapon->name = "Baton en bois";
    printf("%s\n",startWeapon->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *startWeapon);

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




    //FIN PARTIE TEST WEAPONS


    return p_player;
}


int delete_player (st_player* p_player) //Supprime le joueur.
{
    //débug de vérification du free de l'ensemble des armes du joueur
    //je le fais ici car les armes ont besoin d'être free seulement lorsque le joueur meurt.
    deleteWeaponsPlayer(p_player->weapons);

    printf("debug :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return 0;
}

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque : %d \n", p_player->attack );

    WeaponsPlayer *tmp = p_player->weapons;
    while (tmp != NULL) {
        if (tmp->weapon.isEquipped == 1) {
            printf("Arme equipee : %s\n", tmp->weapon.name);
            break;
        }
        tmp = tmp->next;
    }

    return 0;
}