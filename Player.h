#ifndef DOOMDEPTHSC_G21_PLAYER_H
#define DOOMDEPTHSC_G21_PLAYER_H

typedef struct {
    int maxLife; //stats player
    int currentLife;
    int nb_attack_per_round;
    int attack;
    int defense;
    int weapons; // weapons à revoir, j'ai mis ca comme ça histoire de, mais surement une structure rien que pour les armes, comme il y'a plusieurs armes, avec des statistiques différentes etc...
} st_player;

st_player *create_player(void);

st_player *delete_player(st_player *p_player);

int display_player(st_player *p_player);

#endif //DOOMDEPTHSC_G21_PLAYER_H
