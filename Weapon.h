#ifndef DOOMDEPTHSC_G21_WEAPON_H
#define DOOMDEPTHSC_G21_WEAPON_H

typedef struct Weapon {
    char* name;
    int nb_attack_per_round;
    int minDamage;
    int maxDamage;
    int isEquipped;
}Weapon;

typedef struct WeaponsPlayer {
    int id;
    Weapon weapon;
    struct WeaponsPlayer *next;
}WeaponsPlayer;

Weapon* createWeapon(char* name, int nb_attack_per_round, int minDamage, int maxDamage);
void deleteWeapon(Weapon *weapon);
void displayWeapon(Weapon* weapon);

WeaponsPlayer* addWeaponToWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon);
void deleteWeaponsPlayer(WeaponsPlayer *weapons);
int countWeaponsPlayer(WeaponsPlayer *weapons);

#endif //DOOMDEPTHSC_G21_WEAPON_H