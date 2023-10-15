#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Weapon.h"

void displayWeapon(Weapon* weapon)
{
    if(weapon->canBeDisplayed == 1){
        printf("Weapon - %s (damage : %d-%d, attacks per turn: %d)", weapon->name, weapon->minDamage, weapon->maxDamage, weapon->nb_attack_per_round);
        if(weapon->isEquipped == 1){
            printf(" (Equipped)\n");
        } else{
            printf("\n");
        }
    }
}