//
// Created by ethan on 14/10/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"


//-------------- FIGHT -----------------------------
// le FIGHT.
st_monsters *fight_player_round (st_player *p_player, st_monsters *p_first_monster)
{
    st_monsters *p_monster_found; // le monstre que l'on veut attaquer.

    p_monster_found = searchMonster(p_first_monster); // on utilise la func pour choisir le monstre, et on stock ce monstre.
    if (p_monster_found == NULL) // si marche pas
    {
        printf("Monstre non trouvé. \n");
        return p_first_monster; // on renvoit le le first.
    }

    p_monster_found->currentLife -= p_player->attack; // opération, on diminue la vie du monstre, en fonction de l'attaque du joueur.

    if (p_monster_found->currentLife < 0)
        p_monster_found->currentLife = 0; // à partir du moment ou la vie du monstre < 0, on la remet à zéro. (au lieu d'avoir [random] -8, on met 0, plus clean.

    if (p_monster_found->currentLife == 0) { // si la vie du monstre == 0
        printf("Le monstre %d est mort !\n", p_monster_found->number);
        return delete_the_monster(p_first_monster, p_monster_found); //on le sup.
    }
    else
    {
        printf("Le monstre %d vient de prendre %d (%d/%d) \n", p_monster_found->number,
               p_player->attack, p_monster_found->currentLife, p_monster_found->maxLife); //sinon il prend simplement les dégats.
        return p_first_monster; // et on retourne le premier monstre.
    }
}

st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster) {

    st_monsters *p_monster = p_first_monster;
    while (p_monster != NULL) { // tant que y'a des monstres :
        if (p_monster->currentLife > 0) { // et que le monstre est vivant ;
            p_player->currentLife -= p_monster->attack; // monstre attack
            if (p_player->currentLife < 0)
                p_player->currentLife = 0; // on remet à 0 pour pas avoir de valeurs négatives
            printf("Le monstre %d inflige %d dammage au player (life :%d/%d)\n", p_monster->number, p_monster->attack,
                   p_player->currentLife, p_player->maxLife);
        }
        p_monster = (st_monsters *) p_monster->p_next; // i++
    }

    if (p_player->currentLife == 0) { // si la vie du player == 0
        printf("GAME OVER"); // ici faudras appeller une fonction qui pop un giga message.
        return delete_player(p_player); //on le sup.
    }
    return p_player;
}


