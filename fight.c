//
// Created by ethan on 14/10/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "Weapon.h"
#include "Armor.h"
#include "level.h"
#include "Sort.h"
#include "Display.h"

//-------------- FIGHT -----------------------------
// le FIGHT.
st_monsters *fight_player_round (st_player *p_player, st_monsters *p_first_monster, int nb_level)
{
    st_monsters *p_monster_found; // le monstre que l'on veut attaquer.

    srand(time(NULL));

    p_monster_found = searchMonster(p_first_monster); // on utilise la func pour choisir le monstre, et on stock ce monstre.
    if (p_monster_found == NULL) // si marche pas
    {
        printf("Monstre non trouve. \n");
        return p_first_monster; // on renvoit le first.
    }

    int randomDamage = rand() % (p_player->maxAttack - p_player->minAttack + 1) + p_player->minAttack;
    p_monster_found->currentLife -= randomDamage; // opération, on diminue la vie du monstre, en fonction de l'attaque du joueur.

    if (p_monster_found->currentLife < 0)
        p_monster_found->currentLife = 0; // à partir du moment ou la vie du monstre < 0, on la remet à zéro. (au lieu d'avoir [random] -8, on met 0, plus clean.

    if (p_monster_found->currentLife == 0) { // si la vie du monstre == 0
        printf("Le monstre %d est mort !\n", p_monster_found->number);

        if ( p_monster_found->percentGainMana > 50){
            int manaAmount = rand() % 21 + 10;
            p_player->currentMana += manaAmount;
            if ( p_player->currentMana > p_player->maxMana)
                p_player->currentMana = p_player->maxMana;

            printf("Vous avez gagnez %d points de mana\n Mana: %d / %d\n", manaAmount, p_player->currentMana, p_player->maxMana);
        }

        if ( p_monster_found->percentGainGold > 50){
            int goldAmount = rand() % 21 + 15;
            p_player->gold += goldAmount;
            printf("Vous avez gagnez %d pieces d'or\n Or: %d\n", goldAmount, p_player->gold);
        }

        //Test de drop d'equipement dès que le monstre a été tué.
        int dropChance = rand() % 100 + 1;
        if (dropChance <= 25) {
            int whatEquipment = rand() % 2 + 1;
            if (whatEquipment == 1) {
                Weapon *newWeapon = createWeapon(nb_level);
                addWeaponsPlayer(p_player->weapons, *newWeapon);
            } else {
                Armor *newArmor = createArmor(nb_level);
                addArmorsPlayer(p_player->armors, *newArmor);
            }
        }

        return delete_the_monster(p_first_monster, p_monster_found); //on le sup.
    }
    else
    {
        printf("Le monstre %d vient de prendre %d (%d/%d) \n", p_monster_found->number,
               randomDamage, p_monster_found->currentLife, p_monster_found->maxLife); //sinon il prend simplement les dégats.
        return p_first_monster; // et on retourne le premier monstre.
    }
}

st_monsters *sort_player_round (st_player *p_player, st_monsters *p_first_monster, Sort sort) {

    st_monsters *p_monster_found;

    if (sort.type == OFFENSIVE) {

        p_monster_found = searchMonster(p_first_monster);
        if (p_monster_found == NULL) {
            printf("Monstre non trouve. \n");
            return p_first_monster;
        }

        p_monster_found->currentLife -= sort.damage;
        p_player->currentMana -= sort.resources;

        if (p_monster_found->currentLife < 0)
            p_monster_found->currentLife = 0;

        if (p_monster_found->currentLife == 0) {
            printf("Le monstre %d est mort !\n", p_monster_found->number);

            if (p_monster_found->percentGainMana > 50) {
                int manaAmount = rand() % 21 + 10;
                p_player->currentMana += manaAmount;
                if (p_player->currentMana > p_player->maxMana)
                    p_player->currentMana = p_player->maxMana;

                printf("Vous avez gagnez %d points de mana\n Mana: %d / %d\n", manaAmount, p_player->currentMana,
                       p_player->maxMana);
            }

            if (p_monster_found->percentGainGold > 50) {
                int goldAmount = rand() % 21 + 15;
                p_player->gold += goldAmount;
                printf("Vous avez gagnez %d pieces d'or Or: %d\n", goldAmount, p_player->gold);
            }

            return delete_the_monster(p_first_monster, p_monster_found); //on le sup.
        } else {
            printf("Le monstre %d vient de prendre %d (%d/%d) \n", p_monster_found->number,
                   sort.damage, p_monster_found->currentLife, p_monster_found->maxLife);
            return p_first_monster;
        }
    }else {
        printf("Type incorrecte");
    }

    return p_first_monster;

}


st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster) {
    st_monsters *p_monster = p_first_monster;
    while (p_monster != NULL) { // tant que y'a des monstres :
        if (p_monster->currentLife > 0) { // et que le monstre est vivant ;
            int damageTaken = p_monster->attack - p_player->defense;
            damageTaken = (damageTaken < 1) ? 1 : damageTaken;
            p_player->currentLife -= damageTaken; // monstre attack
            if (p_player->currentLife < 0)
                p_player->currentLife = 0; // on remet à 0 pour pas avoir de valeurs négatives
            printf("Le monstre %d inflige %d damage au player (life :%d/%d)\n", p_monster->number, p_monster->attack,
                   p_player->currentLife, p_player->maxLife);
        }
        p_monster = (st_monsters *) p_monster->p_next; // i++
    }

    if (p_player->currentLife == 0) { // si la vie du player == 0
        gameOver(); // ici faudras appeller une fonction qui pop un giga message.
        sleep(5);
        return delete_player(p_player); //on le sup.
    }
    return p_player;
}


