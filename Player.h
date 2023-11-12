#ifndef DOOMDEPTHSC_G21_PLAYER_H
#define DOOMDEPTHSC_G21_PLAYER_H

typedef struct st_player
{
    int maxLife; //stats player
    int currentLife;
    int attack;
    int defense;
   // WeaponsPlayer *weapons;
    int* weapons;
    int* armors;
} st_player;

st_player *create_player(int id_db);

st_player *delete_player(st_player *p_player);

int display_player(st_player *p_player);

#endif //DOOMDEPTHSC_G21_PLAYER_H
