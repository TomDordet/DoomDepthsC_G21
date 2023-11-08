#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"
#include "sqlite3.h"



// PROTOTYPE FONCTIONS (pas issue de .h donc on les protos ici)
// FIGHT
st_monsters *fight_player_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func joueur attaque monstre
st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func monstre(s) attaque joueur


//HEAL
int heal(st_player *p_player); //proto heal

// --- BDD ----


int create_tables(sqlite3 *db);
int init_bdd(void);
int insertData(st_player* p_player, st_level* p_level);

// LOAD SAVE
int displayLatestSaves(void);


// --------------- DISPLAY ENTRE LVL ---------------------


// Menu à la fin d'un niveau.
int postLvlMenu(void)
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
            case 2:
                return 2;
            case 3:
                return 3;
            case 4:
                return 4;
        }
    }
}


//EXIT
int exit_game (st_player *p_player)
{
    //FREE tous les levels (avec les monstres)
    delete_all_level ();
    //FREE le joueur
    delete_player(p_player);
    return 0;
}


// LA GAME
int game(int id_save) // prend en param un id_save, qui sert à savori si on as load une sauvegarde ou non.
{

    //printf("DEBUG --- %s ----id_db %d \n", __FUNCTION__ ,id_save);

    // création du joueur (avec id_save, pour récup les stats d'une save)
    st_player *p_player = create_player(id_save);
    // et init des levels (avec les monstres, et aussi basé sur une sauvegarde ou non)
    init_level(id_save);


    int choixMenu = 1;
    while (1)
    {
        printf("--- LEVEL %d ----\n", get_lvl());
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.heal \n");
        printf("5.sortir \n");
        scanf("%d", &choixMenu);

        switch (choixMenu)
        {
            // STATS JOUEUR
            case 1:
            {
                //affichage des statistiques du joueur.
                display_player(p_player);
                break;
            }
            //STATS DES MONSTRES
            case 2:
            {

                printf("-- Level %d -- \n",get_lvl());
                display_monsters(get_lvl_monsters(get_lvl())); // affiche les monstres.

                break;
            }
            // ROUND
            case 3:
            {
                // ATTAQUE DU JOUEUR - prend en param le joueur, les monstres du niveau, et le niveau
                st_monsters * p_fight = fight_player_round(p_player, get_lvl_monsters(get_lvl()));

                // si le res de p_fight == NULL, alors c'est que les monstres sont tous morts.
                if (p_fight == NULL)
                {
                    //variable qui contient le retour du menu "postLvlMenu"
                    int f_menu = 0;

                    // on remet à NULL les monstres dans le lvl
                    set_lvl_monsters(NULL, get_lvl());
                    printf("Tout les monstres sont morts !");

                    f_menu = postLvlMenu();
                    if (f_menu == 2)
                    {
                        // si on choisis de sauvegarder, on inserts les datas du joueur, des levels et des monstres.
                        insertData(p_player, g_st_level);
                        next_level(); // et on passe au lvl suivant
                    }
                    else if (f_menu == 3)
                    {
                        //on continue.
                        next_level();
                    }
                    else
                    {
                        int var;
                        printf("Tout sera supprimer, continuer ? (1/0)\n");
                        scanf("%d", &var);
                        if (var == 1)
                        {
                            return exit_game (p_player);
                        }
                    }
                    break;
                }
                else if (p_fight != get_lvl_monsters(get_lvl()))
                {
                    set_lvl_monsters(p_fight, get_lvl());
                }

                // attaque du joueur.
                p_player = fight_monsters_round(p_player, get_lvl_monsters(get_lvl())); // déclenche l'attaque du monstre.
                if (NULL == p_player) // si joueur = NULL (donc mort...)
                {
                    printf("Vous avez perdu, retour au menu... \n ");
                    /*Libere tous les montres*/
                    delete_all_level ();
                    return 0; // ?
                }
                break;
            }
            case 4:
                // heal du joueur.
                heal(p_player);
                break;
            case 5:
            {
                // leave.
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
}
// ---------------- WELCOME ----------------

// premier menu jeu.
int welcome(void)
{
    int choixMenu = 0;
    int id_save = 0;
    while (choixMenu != 3)
    {
        printf("--DOOMDEPTH-- \n\n");
        printf("1 - Start Game \n");
        printf("2 - Choisir une partie \n");
        printf("3 - Exit\n");
        scanf("%d", &choixMenu);

        switch (choixMenu) {
            case 1:
            {
                // On lance la game, avec l'id de la sauvegarde en paramètre. Par défault l'id est à 0, donc si on lance sans choisir, on lance sans sauvegarde.
                game(id_save);
                break;
            }
            case 2:
            {
                // Affichage des sauvegardes disponibles (limité aux 5 dernières).
                id_save = displayLatestSaves();
                //printf("ID DB is %d\n", id_db);
                break;
            }
            case 3: {
                break;
            }
        }
    }
    return 0;
}

int main(void)
{
    // Créer la BD, si déjà créer, ne fais rien.
    init_bdd();
    // on lance le premier menu.
    welcome();
}
