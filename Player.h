#ifndef DOOMDEPTHSC_G21_PLAYER_H
#define DOOMDEPTHSC_G21_PLAYER_H

typedef struct st_player
{
    int maxLife;
    int currentLife;
    int nb_attack_per_round;
    int attack;
    int defense;
    int weapons;
} st_player;

st_player* create_player ();
int delete_player (st_player * p_player);
int display_player (st_player* p_player);

#endif //DOOMDEPTHSC_G21_PLAYER_H
