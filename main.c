#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Monster.h"
#include "Weapon.h"

#define MAX_MONSTER 5 // valeur "interne" au proramme

//-----------------PARTIE FIGHT-----------------
st_monsters * searchMonster(st_monsters* p_first_monster) // func pour chercher un monstre (pour choisir celui qu'on va attaquer). Elle a été faite pour ca, mais il y'a possibilité que l'on soit amené à l'utilsier pour autre chose, donc son contenu pourra changer.
{
    int nb; //number du monstre choisis.
    int maxNb = 0; //
    st_monsters* p_monster = p_first_monster; // p_monster contient notre first monster.
    while(p_monster != NULL) { // parcours de la liste chainé.
        printf("Monstre %d (%d/%d) \n", p_monster->number, p_monster->currentLife, p_monster->maxLife); // affiche chaque monstre.
        if(p_monster->number > maxNb){ // si le ->number du monstre est sup à MaxNB ;
            maxNb = p_monster->number; // maxNb = le ->number du monstre. (Ici maxNb est un compteur, qui nous permet de savoir le nombre de monstre, comme à chaque fois le nombre peut varié.
        }
        p_monster = (st_monsters  *)p_monster->p_next; // i++.
    }

    printf("Lequel ? :"); // on demande lequel on veut attaquer.
    scanf("%d", &nb);

    p_monster = p_first_monster; // on remet p_monster au premier monstre.

    while(p_monster != NULL){ // parcours de la liste etc..
        if(p_monster->number == nb) { //
            return p_monster; // si le monstre->number correspond au num choisis, on le retourne (comme une crêpe).
        }
        p_monster = (st_monsters *)p_monster->p_next; // i++
    }
    return NULL; // sinon on return null, la faudrait générer une erreur, ou un message mais azi.
}


st_monsters * fight_round(st_player* p_player, st_monsters* p_first_monster){ // le FIGHT.

    st_monsters * p_monster_found; // le monstre que l'on veut attaquer.
    p_monster_found = searchMonster(p_first_monster); // on utilise la func pour choisir le monstre, et on stock ce monstre.
    if(p_monster_found == NULL) // si marche pas
    {
        printf("No one found. \n");
        return p_first_monster; // on renvoit le le first.
    }

    p_monster_found->currentLife -= p_player->attack; // opération, on diminue la vie du monstre, en fonction de l'attaque du joueur.

    if(p_monster_found->currentLife < 0) p_monster_found->currentLife = 0; // à partir du moment ou la vie du monstre < 0, on la remet à zéro. (au lieu d'avoir [random] -8, on met 0, plus clean.

    if(p_monster_found->currentLife == 0){ // si la vie du monstre == 0
        printf("Monstre %d éliminé \n", p_monster_found->number);
        return delete_the_monster(p_first_monster, p_monster_found); //on le sup.
    }
    else{
        printf("Le monstre %d vient de prendre %d , sa vie est desormais de %d \n",p_monster_found->number, p_player->attack, p_monster_found->currentLife); //sinon il prend simplement les dégats.
        return p_first_monster; // et on retourne le premier monstre.
    }
}
//-----------------FIN PARTIE FIGHT-----------------


//-----------------PARTIE INVENTAIRE-----------------

void display_inventory(st_player* p_player){
    int option = 1;

    while (option != 4){
        printf("1. Armes\n");
        printf("2. Armures\n");
        printf("3. Potions\n");
        printf("4. Retour\n");
        scanf("%d", &option);

        switch (option) {
            case 1: //Inventaire -> Armes
                displayWeaponsPlayer(p_player->weapons);
                int weaponOption = 1;

                while (weaponOption != 2) {
                    printf("1. Changer d'arme\n");
                    printf("2. Retour\n");
                    scanf("%d", &weaponOption);

                    int equipWeapon = -1;
                    switch(weaponOption){
                        case 1: //Armes -> Changer d'arme
                            displayWeaponsPlayer(p_player->weapons);
                            printf("Quelle arme equiper?\n");
                            scanf("%d", &equipWeapon);
                            if (equipWeapon == -1){
                                break;
                            }
                            else if (equipWeapon > countWeaponsPlayer(p_player->weapons)){
                                printf("Erreur de saisie : aucune arme ne correspond au numero %d\n", equipWeapon);
                                break;
                            }
                            else {
                                changeIsEquippedToWeaponsPlayer(p_player->weapons, equipWeapon);
                            }
                        case 2: //Armes -> Retour
                            break;
                    }
                }
                break;
            case 2: //Inventaire -> Armures
                printf("Vous accedez aux armures\n");
                break;
            case 3: //Inventaire -> Potions
                printf("Vous accedez aux potions\n");
                break;
            case 4: //Inventaire -> Retour
                break;
        }
    }
}
//-----------------FIN PARTIE INVENTAIRE-----------------



int main(void)
{

    int nb_monster = 0;
    int choixMenu = 1;

    st_player* p_player = create_player(); // on créer le joueur.
    st_monsters * p_first_monster = create_monsters(&nb_monster); //on créer nos monstres.

    while (choixMenu != 4)
    {
        printf("---Menu-- - \n");
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.sortir \n");
        printf("5.Inventaire\n");
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
                p_first_monster = fight_round(p_player, p_first_monster);
                break;
            case 4:
                break;
            case 5:
                display_inventory(p_player);
                break;
        }
    }

    //Libere les montres*/
    delete_monsters(p_first_monster);
    //Libère le joueur.
    delete_player(p_player);

    //system("PAUSE");

    return 0;
}
