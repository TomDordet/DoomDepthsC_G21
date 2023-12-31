#ifndef DOOMDEPTHSC_G21_WEAPON_H
#define DOOMDEPTHSC_G21_WEAPON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Weapon {
    char *name;
    int nb_attack_per_round;
    int minDamage;
    int maxDamage;
    int isEquipped;
}Weapon;

typedef struct WeaponsPlayer {
    Weapon weapon;
    struct WeaponsPlayer *next;
}WeaponsPlayer;

Weapon* createWeapon(int level);
void deleteWeapon(Weapon *weapon);
void displayWeapon(Weapon* weapon);


WeaponsPlayer* addWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon);
void deleteWeaponsPlayer(WeaponsPlayer *weapons);
int countWeaponsPlayer(WeaponsPlayer *weapons);
WeaponsPlayer* getWeaponNumberToWeaponsPlayer(WeaponsPlayer* weaponsList, int number);
void displayWeaponsPlayer(WeaponsPlayer *weapons);
void swapWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon);
//void swapWeaponsPlayer(st_player *p_player, Weapon weapon);
void changeIsEquippedToWeaponsPlayer(WeaponsPlayer *weapons, int numberWeapon);


char* generateRandomWeaponName();
#endif //DOOMDEPTHSC_G21_WEAPON_H