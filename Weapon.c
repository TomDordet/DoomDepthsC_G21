#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Weapon.h"
//PARTIE WEAPON

Weapon* createWeapon(char* name, int nb_attack_per_round, int minDamage, int maxDamage) {
    Weapon *newWeapon = malloc(sizeof(Weapon));
    if (newWeapon == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    newWeapon->name = name;
    newWeapon->nb_attack_per_round = nb_attack_per_round;
    newWeapon->minDamage = minDamage;
    newWeapon->maxDamage = maxDamage;
    newWeapon->isEquipped = 0;
    return newWeapon;
}


void deleteWeapon(Weapon *weapon){
    free(weapon);
}

void displayWeapon(Weapon *weapon)
{
    printf("Weapon - %s (damage : %d-%d, attacks per turn: %d)", weapon->name, weapon->minDamage, weapon->maxDamage, weapon->nb_attack_per_round);
    if(weapon->isEquipped == 1){
        printf(" (Equipped)");
    }
    printf("\n");
}


//PARTIE LISTE CHAINEES DE WEAPONS

WeaponsPlayer* addWeapon(WeaponsPlayer *weapons, Weapon weapon){
    WeaponsPlayer *newWeapon = malloc(sizeof(WeaponsPlayer));
    if (newWeapon == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    newWeapon->weapon = weapon;
    newWeapon->next = NULL;

    if (weapons == NULL){
        return newWeapon;
    }

    WeaponsPlayer *findEnd = weapons;
    while (findEnd->next != NULL){
        findEnd = findEnd->next;
    }

    findEnd->next = newWeapon;
    return weapons;
}

void deleteWeaponsPlayer(WeaponsPlayer *weapons){
    while (weapons != NULL){
        WeaponsPlayer *tmp = weapons;
        weapons = weapons->next;
        deleteWeapon((&tmp->weapon));
        free(tmp);
    }
}