//
// Created by Tom on 06/11/2023.
//

#ifndef DOOMDEPTHSC_G21_ARMOR_H
#define DOOMDEPTHSC_G21_ARMOR_H

typedef struct Armor
{
    char *name;
    int defense;
    int isEquipped;
    int *next;
}Armor;

Armor* createArmor(int defense);
Armor* createArmorSave(int defense, const char* name);
void deleteArmor(Armor *armor);
void displayArmor(Armor* armor);

int* addArmorsPlayer(st_player * player, Armor* New_Armor);
void deleteArmorsPlayer(st_player * player);
int countArmorsPlayer(st_player * player);
Armor* getArmorNumberToArmorsPlayer(st_player * player, int number);
void displayArmorsPlayer(int* armors);
void swapArmorsPlayer(st_player *p_player, Armor newArmor);
void changeIsEquippedToArmorsPlayer(st_player* p_player, int numberArmor);

char* generateRandomArmorName();


#endif //DOOMDEPTHSC_G21_ARMOR_H
