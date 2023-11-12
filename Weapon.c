#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Weapon.h"


//PARTIE WEAPON

Weapon *createWeapon(int nb_attack_per_round, int minDamage, int maxDamage) {
    Weapon *newWeapon = malloc(sizeof(Weapon));

    char *randomName = generateRandomWeaponName();
    newWeapon->name = randomName;

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
    printf("Weapon - %s (damage: %d-%d, attacks per turn: %d)", weapon->name, weapon->minDamage, weapon->maxDamage, weapon->nb_attack_per_round);
    if(weapon->isEquipped == 1){
        printf(" (Equipped)");
    }
    printf("\n");
}


//PARTIE LISTE CHAINEES DE WEAPONS

WeaponsPlayer* addWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon) {
    int count = countWeaponsPlayer(weapons);
    if (count == 5) {
        swapWeaponsPlayer(weapons, weapon);
        return weapons;
    }

    WeaponsPlayer *newWeapon = malloc(sizeof(WeaponsPlayer));
    newWeapon->weapon = weapon;
    newWeapon->next = NULL;

    //debug
    if (weapons == NULL){
        printf("weapons == null\n");
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
        printf("debug :: suppression arme");
    }
}

int countWeaponsPlayer(WeaponsPlayer *weapons){
    int count = 0;
    WeaponsPlayer *tmp = weapons;

    while (tmp != NULL){
        count++;
        tmp = tmp->next;
    }
    return count;
}

WeaponsPlayer* getWeaponNumberToWeaponsPlayer(WeaponsPlayer *weapons, int number) {
    WeaponsPlayer* currentWeapon = weapons;
    int currentNumber = 1;

    while (currentWeapon != NULL) {
        if (currentNumber == number) {
            return currentWeapon;
        }

        currentWeapon = currentWeapon->next;
        currentNumber++;
    }
    return NULL;
}

void displayWeaponsPlayer(WeaponsPlayer *weapons) {
    printf("----------\n");
    printf("Armes :\n");
    WeaponsPlayer *tmp = weapons;
    int value = 1;
    while (tmp != NULL){
        printf("%d - ", value);
        displayWeapon(&tmp->weapon);
        tmp = tmp->next;
        value++;
    }
    printf("----------\n");
}


void swapWeaponsPlayer(WeaponsPlayer *weapons, Weapon newWeapon) {
    displayWeaponsPlayer(weapons);

    printf("Selectionnez le numero de l'arme que vous souhaitez remplacer : ");
    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= countWeaponsPlayer(weapons)) {
        WeaponsPlayer *weaponToSwap = getWeaponNumberToWeaponsPlayer(weapons, choice);

        if (weaponToSwap->weapon.isEquipped) {
            weaponToSwap->weapon.isEquipped = 0;
            newWeapon.isEquipped = 1;
            //changeMinAndMaxAttackValues(p_player);
        }

        weaponToSwap->weapon = newWeapon;
        printf("Vous avez remplace l'arme selectionnee par :\n");
        displayWeapon(&newWeapon);
    } else if (choice == 9){
        printf("Vous avez decide de garder vos armes actuelles.\n");
    } else {
        printf("Choix invalide. Aucune arme n'a ete remplacee.\n");
    }
}

void changeIsEquippedToWeaponsPlayer(WeaponsPlayer *weapons, int numberWeapon) {
    WeaponsPlayer* currentWeapon = weapons;
    int currentNumber = 1;


    while (currentWeapon != NULL) {
        if (currentNumber == numberWeapon) {
            currentWeapon->weapon.isEquipped = 1;
            printf("Vous avez equipe '%s'\n", currentWeapon->weapon.name);
        } else {
            currentWeapon->weapon.isEquipped = 0;
        }
        currentWeapon = currentWeapon->next;
        currentNumber++;
    }
}

//PARTIE RANDOM NAME WEAPON

char* weaponNames[] = {
        "Epee de feu",
        "Arc de Glace",
        "Lance des Ombres",
        "Katana Enchante",
        "Couteau Empoisonne",
        "Baton de Foudre",
        "Hache de Guerre en Fer",
        "Marteau de Tonnerre",
        "Fleau de Poison",
        "Dague de l'Ombre",
        "Epee de Cristal",
        "Arc Celeste",
        "Lance du Dragon",
        "Katana de l'Aube",
        "Poignard de l'Empereur",
        "Baton de Sagesse",
        "Hache de Lave",
        "Marteau de Givre",
        "Fleau de la Nuit",
        "Dague des Abysses",
        "Epee de l'Aigle",
        "Arc de la Lune",
        "Lance de l'Etoile Filante",
        "Katana de la Tempete",
        "Couteau du Chaos",
        "Baton des Anciens",
        "Hache de la Foudre",
        "Marteau du Geant",
        "Fleau du Demon",
        "Dague des Tenebres",
        "Epee du Paladin",
        "Arc du Chasseur",
        "Lance de la Justice",
        "Katana du Maitre",
        "Couteau du Voleur",
        "Baton du Mage",
        "Hache de l'Inferno",
        "Marteau de la Colere",
        "Fleau de la Pestilence",
        "Dague du Bandit",
        "Epee de la Vengeance",
        "Arc de la Discorde",
        "Lance de la Ruine",
        "Katana de l'Honneur",
        "Couteau de l'Assassin",
        "Baton de l'Illumination",
        "Hache de la Terreur",
        "Marteau du Jugement",
        "Fleau de la Redemption",
        "Master Sword"
};

char* generateRandomWeaponName() {
    int randomIndex = rand() % (sizeof(weaponNames) / sizeof(weaponNames[0]));
    return weaponNames[randomIndex];
}