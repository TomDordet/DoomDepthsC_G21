#ifndef DOOMDEPTHSC_G21_MONSTER_H
#define DOOMDEPTHSC_G21_MONSTER_H

typedef struct
{
    int number; // numero du monstre (comme le nombre de monstre est aléatoire, on lui donne un nombre, pour la suite.

    int maxLife; //stats monstres
    int currentLife;
    int attack;
    int defense;

    int * p_next; // Pointeur d'int pour passer de monstre en monstre, voir suite.
} st_monsters;

st_monsters * create_monsters (int * p_nb_monster);
int delete_monsters (st_monsters * p_monster);
st_monsters * delete_the_monster (st_monsters * p_first_monster, st_monsters* p_the_monster);
int display_monsters (st_monsters* p_first_monster);
st_monsters * searchMonster(st_monsters* p_first_monster);

#endif //DOOMDEPTHSC_G21_MONSTER_H
