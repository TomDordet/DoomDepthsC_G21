#ifndef DOOMDEPTHSC_G21_WEAPON_H
#define DOOMDEPTHSC_G21_WEAPON_H

#include "sqlite3.h"
#include "Player.h"

typedef struct Weapon
{
    char *name;
    int nb_attack_per_round;
    int minDamage;
    int maxDamage;
    int isEquipped;
    int *next;
}Weapon;

Weapon *createWeapon(int nb_attack_per_round, int minDamage, int maxDamage);
Weapon *createWeaponSave(int nb_attack_per_round, int minDamage, int maxDamage, const char* name);

void deleteWeapon(Weapon *weapon);
void displayWeapon(Weapon* weapon);


int* addWeaponsPlayer(st_player *player, Weapon* weapon);

void deleteWeaponsPlayer(st_player * p_player);
int countWeaponsPlayer(st_player * player);
Weapon* getWeaponNumberToWeaponsPlayer(st_player * player, int number);
void displayWeaponsPlayer(int *weapons);
void swapWeaponsPlayer(st_player * player, Weapon weapon);
void changeIsEquippedToWeaponsPlayer(st_player * player, int numberWeapon);


char* generateRandomWeaponName();
#endif //DOOMDEPTHSC_G21_WEAPON_H