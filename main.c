#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
//#include "Monster.h"
#include "level.h"

st_monsters *fight_player_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func joueur attaque monstre
st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func monstre(s) attaque joueur
int heal(st_player *p_player); //proto heal
int next_game(void); // proto
int save_lvl(void); // proto save LVL (à venir)


// --------------- DISPLAY ENTRE LVL ---------------------

// les SALLES
/*
int next_game(void){
    int game_choice = 1;

    while(1)
    {
        printf("Trois portes se trouvent devant vous, laquel voulez-vous empruntez ? \n");
        printf("1 - A gauche\n");
        printf("2 - Tout droit\n");
        printf("3 - A droite\n");
        scanf("%d", &game_choice);

        switch(game_choice){
            case 1: {
                return next_level();
            }
            case 2: {
                return next_level();
            }
            case 3: {
                return next_level();
            }
        }
    }
}*/

int first_menu(void)
{
    while(1)
    {
        int save_choice = 1;
        printf("Quelques minutes de pause avant de reprendre... Que voulez vous faire ? \n");
        printf("1 - Inventaire\n");
        printf("2 - Sauvegarde\n");
        printf("3 - Continuer\n");
        printf("4 - Exit\n");

        scanf("%d", &save_choice);

        switch(save_choice){
            case 1: {
                printf("inventaire à venir..\n");
                break;
            }
            case 2: {
                printf("Sauvegarde à venir..\n");
                break;
            }
            case 3:
                return 3;
            case 4:
                return 4;
        }
    }
}

// ---- LA GAME

//fin de game
int exit_game (st_player *p_player)
{
    //Libere tous les montres
    delete_all_level ();
    //libere le joueur
    delete_player(p_player);
    return 0;
}

int game(void)
{
    int choixMenu = 1;

    st_player *p_player = create_player(); // on créer le joueur.
    init_level(); // init le lvl (qui init la créa des mstr)

    while (1)
    {
        printf("--- LEVEL %d ----\n\n", get_lvl());
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.heal \n");
        printf("5.sortir \n");
        scanf("%d", &choixMenu);

        switch (choixMenu)
        {
            case 1: {
                display_player(p_player); // affiche les stats du joueur.
                break;
            }
            case 2:
            {
                printf("-- Level %d -- \n",get_lvl()); // affiche le LVL actuel
                display_monsters(get_lvl_monsters(get_lvl())); // affiche les monstres.
                break;
            }
            case 3:
            {
                /*Game*/
                st_monsters * pNew = fight_player_round(p_player, get_lvl_monsters(get_lvl()));
                // Si le retour de fight_player_round == NULL, alors monstres morts / désallouer (voir ce que la fonction retourne à sa définition, pour comprendre).
                if (pNew == NULL)
                {
                    int f_menu;

                    // on remet à NULL les monstres dans le lvl, pck ils sont morts.
                    set_lvl_monsters(NULL, get_lvl());
                    printf("Tout les monstres sont morts !");
                    f_menu = first_menu();
                    if (f_menu == 3){//next game, next level
                        //next_game();
                        next_level();
                    }
                    else if(f_menu == 4)//Exit
                    {
                        return exit_game (p_player);
                    }
                    break;
                }
                else if (pNew != get_lvl_monsters(get_lvl()))
                {
                    set_lvl_monsters(pNew, get_lvl());
                }
                else
                {
                    //pNew == get_lvl_monsters(get_lvl()
                    //donc on fait rien
                }

                p_player = fight_monsters_round(p_player, get_lvl_monsters(get_lvl())); // déclenche l'attaque du monstre.
                if (NULL == p_player) // si joueur = NULL (donc mort...)
                {
                    printf("Player is dead (has been deleted) .. Gave Over !!!!! \n ");
                    /*Libere tous les montres*/
                    delete_all_level ();
                    return 0; // ?
                }
                break;
            }
            case 4:
                heal(p_player);
                break;
            case 5:
            {
                int var;
                printf("Tout sera supprimer, continuer ? (1/0)\n");
                scanf("%d", &var);
                if (var == 1)
                {
                    return exit_game (p_player);
                }
                break;
            }
        }
    }


    //system("PAUSE");
    return 0;
}



// ---------------- WELCOME ----------------

int welcome(void)
{
    int choixMenu = 0;
    while (choixMenu != 3) {
        printf("--DOOMDEPTH-- \n\n");
        printf("1 - Start New Game \n");
        printf("2 - Load save \n");
        printf("3 - Exit\n");
        scanf("%d", &choixMenu);

        switch (choixMenu) {
            case 1: {
                game();
                break;
            }
            case 2: {
                printf("Bientôt.\n");
                break;
            }
            case 3: {
                break;
            }
        }
    }
    return 0;
}

int main(void) {
    welcome();
}
