//
// Created by ethan on 14/10/2023.
//
#include <stdio.h>

#include "Player.h"

int heal(st_player *p_player) {
    int theHeal = 10;

    if (p_player->currentLife == p_player->maxLife) {
        printf("Vie deja au max.\n");
        return 0;
    }

    if ((p_player->maxLife - p_player->currentLife) < theHeal) {
        p_player->currentLife = p_player->maxLife;
        printf("Vous vous soignez de %d HP, vie au max (%d/%d)\n", theHeal, p_player->currentLife, p_player->maxLife);
        return 0;
    }

    p_player->currentLife += theHeal;
    printf("Vous vous soignez de %d HP (vie : %d/%d)\n", theHeal, p_player->currentLife, p_player->maxLife);
    return 0;
}