#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"
#include "sqlite3.h"

#include "Weapon.h"
#include "Armor.h"


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

//-----------------PARTIE INVENTAIRE-----------------

void display_inventory(st_player* p_player)
{
    int option = 1;

    while (option != 4)
    {
        printf("1. Armes\n");
        printf("2. Armures\n");
        printf("3. Potions\n");
        printf("4. Retour\n");
        scanf("%d", &option);

        switch (option){
            case 1: //Inventaire -> Armes
                displayWeaponsPlayer(p_player->weapons);
                int weaponOption = 1;

                while (weaponOption != 2)
                {
                    printf("1. Changer d'arme\n");
                    printf("2. Retour\n");
                    scanf("%d", &weaponOption);

                    int equipWeapon = -1;
                    switch(weaponOption)
                    {
                        case 1: //Armes -> Changer d'arme
                            displayWeaponsPlayer(p_player->weapons);
                            printf("Quelle arme equiper?\n");
                            scanf("%d", &equipWeapon);
                            if (equipWeapon == -1)
                            {
                                break;
                            }
                            else if (equipWeapon > countWeaponsPlayer(p_player))
                            {
                                printf("Erreur de saisie : aucune arme ne correspond au numero %d\n", equipWeapon);
                                break;
                            }
                            else
                            {
                                changeIsEquippedToWeaponsPlayer(p_player, equipWeapon);
                            }
                        case 2: //Armes -> Retour
                            break;
                    }
                }
                break;
            case 2: //Inventaire -> Armures
                displayArmorsPlayer(p_player->armors);
                int armorOption = 1;

                while (armorOption != 2)
                {
                    printf("1. Changer d'armure\n");
                    printf("2. Retour\n");
                    scanf("%d", &armorOption);

                    int equipArmor = -1;
                    switch (armorOption)
                    {
                        case 1: //Armures -> Changer d'armure
                            displayArmorsPlayer(p_player->armors);
                            printf("Quelle armure equiper?\n");
                            scanf("%d", &equipArmor);
                            if (equipArmor == -1)
                            {
                                break;
                            }
                            else if (equipArmor > countArmorsPlayer(p_player))
                            {
                                printf("Erreur de saisie : aucune arme ne correspond au numero %d\n", equipArmor);
                                break;
                            } else
                            {
                                changeIsEquippedToArmorsPlayer(p_player, equipArmor);
                            }
                        case 2: //Armures -> Retour
                            break;
                    }
                }
            case 3: //Inventaire -> Potions
                printf("Vous accedez aux potions\n");
                break;
            case 4: //Inventaire -> Retour
                break;
        }
    }
}
//-----------------FIN PARTIE INVENTAIRE-----------------

int first_menu(st_player *p_player)
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
                display_inventory(p_player);
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

// ---- LA GAME

//EXIT
int exit_game (st_player *p_player)
{
    //FREE tous les levels (avec les monstres)
    delete_all_level();
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
        printf("5.Inventaire\n");
        printf("9.sortir \n");
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
                    // on remet à NULL les monstres dans le lvl
                    set_lvl_monsters(NULL, get_lvl());
                    printf("Tout les monstres sont morts !");

                    if (MAX_LVL == get_lvl())
                    {
                        printf("Vous avez atteint le sommet !!! \n tous les niveaux ont ete remportés !!! \n C'est la victoir !!!");
                        return exit_game (p_player);
                    }
                    else
                    {
                        //variable qui contient le retour du menu "postLvlMenu"
                        int f_menu = first_menu(p_player);
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
            {
                heal(p_player);
                break;
            }
            case 5:
            {
                display_inventory(p_player);
                break;
            }
            case 9:
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
