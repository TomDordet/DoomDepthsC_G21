#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"

st_monsters * fight_player_round(st_player* p_player, st_monsters* p_first_monster);
st_player * fight_monsters_round(st_player* p_player, st_monsters* p_first_monster);
int heal(st_player * p_player);


#define MAX_MONSTER 5 // valeur "interne" au proramme

//-----------------PARTIE FIGHT-----------------


void display_inventory(st_player* p_player){
    int option = 1;

    while (option != 4){
        printf("1. Armes\n");
        printf("2. Armures\n");
        printf("3. Potions\n");
        printf("4. Retour\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Vous accedez aux armes\n");
                break;
            case 2:
                printf("Vous accedez aux armures\n");
                break;
            case 3:
                printf("Vous accedez aux potions\n");
                break;
            case 4:
                break;
        }
    }
}
//-----------------FIN PARTIE INVENTAIRE-----------------

int game(void)
{
    int nb_monster = 0;
    int choixMenu = 1;

    st_player* p_player = create_player(); // on créer le joueur.
    st_monsters * p_first_monster = create_monsters(&nb_monster); //on créer nos monstres.

    while (1)
    {
        printf("---Menu-- - \n");
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.heal \n");
        printf("5.sortir \n");
        scanf("%d", &choixMenu);

        switch(choixMenu) {
            case 1: {
                display_player(p_player);
                break;
            }
            case 2: {
                display_monsters(p_first_monster);
                break;
            }
            case 3:
                /*Game*/
                p_first_monster = fight_player_round(p_player, p_first_monster);
                p_player = fight_monsters_round(p_player, p_first_monster);
                break;
            case 4:
                heal(p_player);
                break;
            case 5: {
                int var;
                printf("Tout sera supprimer, continuer ? (1/0)\n");
                scanf("%d", &var);
                if (var == 1)
                {
                    //Libere les montres*/
                    delete_monsters(p_first_monster);
                    //Libère le joueur.
                    delete_player(p_player);
                    return 4;
                }
                break;
            }
        }
    }


    //system("PAUSE");
    return 0;
}

int welcome(void)
{
    int choixMenu = 0;
    while (choixMenu != 3)
    {
        printf("--DOOMDEPTH-- \n");
        printf("1 - Start New Game \n");
        printf("2 - Load save \n");
        printf("3 - Leave\n");
        scanf("%d", &choixMenu);

        switch(choixMenu) {
            case 1: {
                game();
                break;
            }
            case 2: {
                printf("Bientôt.");
                break;
            }
            case 3: {
                break;
            }
        }
    }
    return 0;
}

int main(void){
    welcome();
}
