#include "Sort.h"


void sort(st_player* p_player, st_monsters* p_monster, Sort playerChoice){// je comprends plus trop a quoi sert cette fonction en faite

    switch (playerChoice.type) {

        case OFFENSIVE:
            if(p_player->currentMana < playerChoice.resources){
                printf("Pas assez de mana pour ce sort\n");
                break;
            }
            if(playerChoice.sort == DECUPLEMENT){
                p_player->minAttack *= playerChoice.damage;
                p_player->currentMana -= playerChoice.resources;
                printf("Vos attaques sont decuple par %d\n", playerChoice.damage);
                break;
            }
            p_monster->currentLife -= playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;
            printf("Mana actuelle %d / %d\n", p_player->currentMana, p_player->maxMana);
            break;
        case DEFENSIVE:
            if(p_player->currentMana < playerChoice.resources){
                printf("Pas assez de mana pour ce sort\n");
                break;
            }

            p_player->defense += playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;
            printf("Sort defensive activer, %d points\n", playerChoice.damage);
            break;
        case LIFEHEAL:
            if(p_player->currentMana < playerChoice.resources){
                printf("Pas assez de mana pour ce sort\n");
                return;
            }

            p_player->currentLife += playerChoice.damage;
            p_player->currentMana -= playerChoice.resources;

            if ( p_player->currentLife > p_player->maxLife){
                p_player->currentLife = p_player->maxLife;
            }
            printf("Vous avez utilise un sort de regeneration de vie, +%d\n", playerChoice.damage);
            break;
        case MANAHEAL:
            p_player->currentMana += playerChoice.damage;

            if ( p_player->currentMana > p_player->maxMana){
                p_player->currentMana = p_player->maxMana;
            }
            printf("Vous avez utilise un sort de regeneration de mana, +%d\n", playerChoice.damage);
            break;
        default:
            printf("Type incorrecte");
            break;
    }
}

void afficherDescriptionSort(enum Sorts sort) {
    switch (sort) {
        case BOULEDEFEU:
            printf("Boule de feu : Un sort offensif infligeant des degats directs.\n");
            break;
        case ECLAIRFULGURANT:
            printf("Eclair fulgurant : Un sort offensif rapide et puissant.\n");
            break;
        case DECUPLEMENT:
            printf("Decuplement : Un sort offensif qui multiplie la puissance d'attaque du joueur.\n");
            break;
        case MURDECLACE:
            printf("Mur de glace : Un sort defensif qui reduit les attaques ennemies.\n");
            break;
        case BOUCLIERDELUMIERE:
            printf("Bouclier de lumiere : Un sort defensif qui renforce la protection du joueur.\n");
            break;
        case REGENERATIONVIE:
            printf("Regeneration de vie : Un sort de soin qui restaure les points de vie du joueur.\n");
            break;
        case REGENERATIONMANA:
            printf("Regeneration de mana : Un sort de soin qui restaure la reserve de mana du joueur.\n");
            break;
        default:
            printf("Sort inconnu.\n");
    }
}

