#ifndef DOOMDEPTHSC_G21_WEAPON_H
#define DOOMDEPTHSC_G21_WEAPON_H

typedef struct Weapon {
    char* name;
    int nb_attack_per_round;
    int minDamage;
    int maxDamage;
    int isEquipped;
    int canBeDisplayed;
}Weapon;

void displayWeapon(Weapon* weapon);

#endif //DOOMDEPTHSC_G21_WEAPON_H
