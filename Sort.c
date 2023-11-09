#include "Sort.h"


void sort(st_player* p_player, st_monsters* p_monster, Sort playerChoice){

    if(p_player->currentMana <= playerChoice.resources){
        printf("Pas assez de mana pour ce sort");
        return;
    }

    switch (playerChoice.type) {

        case OFFENSIVE:
            p_monster->currentLife -= playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;
            break;
        case DEFENSIVE:
            p_monster->attack -= playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;
            break;
        case LIFEHEAL:
            p_player->currentLife += playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;
            break;
        case MANAHEAL:
            p_player->currentMana += playerChoice.damage;
            break;
        default:
            printf("Type incorrecte");
            break;
    }
}

