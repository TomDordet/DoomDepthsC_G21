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
}Armor;

typedef struct ArmorsPlayer
{
    Armor armor;
    struct ArmorsPlayer *next;
}ArmorsPlayer;

Armor* createArmor(int defense);
void deleteArmor(Armor *armor);
void displayArmor(Armor* armor);

ArmorsPlayer* addArmorsPlayer(ArmorsPlayer *armors, Armor armor);
void deleteArmorsPlayer(ArmorsPlayer *armors);
int countArmorsPlayer(ArmorsPlayer *armors);
ArmorsPlayer* getArmorNumberToArmorsPlayer(ArmorsPlayer *armors, int number);
void displayArmorsPlayer(ArmorsPlayer *armors);
void swapArmorsPlayer(ArmorsPlayer *armors, Armor armor);
void changeIsEquippedToArmorsPlayer(ArmorsPlayer *armors, int numberArmor);

char* generateRandomArmorName();


#endif //DOOMDEPTHSC_G21_ARMOR_H
