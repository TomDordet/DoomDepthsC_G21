#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "sqlite3.h"

//////////////////////

//PARTIE WEAPON

Weapon *createWeapon(int nb_attack_per_round, int minDamage, int maxDamage)
{
    // alloc de l'arme
    Weapon *newWeapon = malloc(sizeof(Weapon));

    // sort un nom de la liste.
    char *randomName = generateRandomWeaponName();
    // new name
    newWeapon->name = randomName;

    newWeapon->nb_attack_per_round = nb_attack_per_round;
    newWeapon->minDamage = minDamage;
    newWeapon->maxDamage = maxDamage;
    newWeapon->isEquipped = 0;
    return newWeapon;
}

// free l'arme
void deleteWeapon(Weapon *weapon)
{
    free(weapon);
}


void displayWeapon(Weapon *weapon)
{
    // affiche l'arme.
    printf("Weapon - %s (damage: %d-%d, attacks per turn: %d)", weapon->name, weapon->minDamage, weapon->maxDamage, weapon->nb_attack_per_round);

    // affiche en + si elle est équipé ou non.
    if(weapon->isEquipped == 1)
    {
        printf(" (Equipped)");
    }
    printf("\n");
}


//PARTIE LISTE CHAINEES DE WEAPONS

WeaponsPlayer* addWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon)
{
    // nb d'armes du joueur.
    int count = countWeaponsPlayer(weapons);
    if (count == 5)
    {
        // si joueur a déjà plus de 5 armes :
        printf("Inventaire d'armes plein !\n");
        return weapons;
    }

    // alloc une nouvelle arme.
    WeaponsPlayer *newWeapon = malloc(sizeof(WeaponsPlayer));
    if (newWeapon == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour WeaponsPlayer\n");
        return weapons; // Ne pas modifier la liste si l'allocation a échoué.
    }

    //la nouvelle arme allouée = l'arme en paramètre.
    newWeapon->weapon = weapon;
    // la suivante on l'a met à NULL (on coupe la liste en gros (?))
    newWeapon->next = NULL;

    // Si la liste est vide, la nouvelle arme devient la première.
    if (weapons == NULL)
    {
        return newWeapon;
    }

    //  findEnd = la nouvelle arme.
    WeaponsPlayer *findEnd = weapons;
    // tant que liste pas finie.
    while (findEnd->next != NULL)
    {
        // nouvelle arme(s) = sa suivante.
        findEnd = findEnd->next;
    }

    // Sa suivante = la nouvelle arme.
    findEnd->next = newWeapon;

    // Retourner la nouvelle tête de la liste.
    return weapons;
}

/*
// new armes
WeaponsPlayer* addWeaponsPlayer(WeaponsPlayer *weapons, Weapon weapon)
{
    // nb d'armes du joueur.
    int count = countWeaponsPlayer(weapons);
    if (count == 5)
    {
        // si joueur a déjà plus de 5 armes :
        printf("Inventaire d'armes plein !\n");
        return weapons;
    }

    // alloc une nouvelle arme.
    WeaponsPlayer *newWeapon = malloc(sizeof(WeaponsPlayer));
    //la nouvelle arme allouer = l'arme en para
    newWeapon->weapon = weapon;
    // la suivante on l'a met à NULL (on coupe la liste en gros (?))
    newWeapon->next = NULL;

    //debug
    if (weapons == NULL)
    {
        printf("weapons == null\n");
        return newWeapon;
    }

    //  findEnd = la nouvelle arme.
    WeaponsPlayer *findEnd = weapons;
    // tant que liste pas finis.
    while (findEnd->next != NULL)
    {
        // nouvelle arme(s) = sa suivante.
        findEnd = findEnd->next;
    }
    // sa suivante = la nouvelle arme.
    // pas sur que ca serve ca ? vu que tu le retourne pas
    findEnd->next = newWeapon;
    return weapons;
}

 */
// free les armes.
void deleteWeaponsPlayer(WeaponsPlayer *weapons)
{
    // parcous toutes les armes.
    while (weapons != NULL)
    {
        WeaponsPlayer *tmp = weapons;
        // sa suivante.
        weapons = weapons->next;
        // et les supprimes
        deleteWeapon((&tmp->weapon));
        free(tmp);
        printf("debug :: suppression arme");
    }
}

// compte les armes que le joueur possède.
int countWeaponsPlayer(WeaponsPlayer *weapons)
{
    int count = 0;
    WeaponsPlayer *tmp = weapons;

    //parcours les armes.
    while (tmp != NULL)
    {
        count++;
        //arme suivante.
        tmp = tmp->next;
    }
    return count;
}

WeaponsPlayer* getWeaponNumberToWeaponsPlayer(WeaponsPlayer *weapons, int number)
{

    WeaponsPlayer* currentWeapon = weapons;
    int currentNumber = 1;
    // parcours les amres
    while (currentWeapon != NULL)
    {
        // si l'arme correspond à celle choisis, on retourne celle choisis.
        if (currentNumber == number)
        {
            return currentWeapon;
        }
        //i++ des armes.
        currentWeapon = currentWeapon->next;
        currentNumber++;
    }
    return NULL;
}

// liste les armes du joueur.
void displayWeaponsPlayer(WeaponsPlayer *weapons)
{
    printf("----------\n");
    printf("Armes :\n");
    WeaponsPlayer *tmp = weapons;
    int value = 1;
    while (tmp != NULL)
    {
        printf("%d - ", value);
        displayWeapon(&tmp->weapon);
        tmp = tmp->next;
        value++;
    }
    printf("----------\n");
}


void swapWeaponsPlayer(WeaponsPlayer *weapons, Weapon newWeapon)
{
    // affiche les armes du joueur.
    displayWeaponsPlayer(weapons);

    printf("Selectionnez le numero de l'arme que vous souhaitez remplacer : ");
    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= countWeaponsPlayer(weapons))
    {
        // parcours les armes du joueur, renvoie l'arme que l'on souhaite remplacer.
        WeaponsPlayer *weaponToSwap = getWeaponNumberToWeaponsPlayer(weapons, choice);

        // si l'arme choisis est équipé, on l'équipe pu..
        if (weaponToSwap->weapon.isEquipped)
        {
            weaponToSwap->weapon.isEquipped = 0;
            // et on équipe la nouvelle.
            newWeapon.isEquipped = 1;
        }
        // on remplace l'ancienne arme par la nouvelle dans l'inventaire du player.
        weaponToSwap->weapon = newWeapon;
        printf("Vous avez remplace l'arme selectionnee par :\n");
        displayWeapon(&newWeapon);
    }
    else if (choice == 9)
    {
        printf("Vous avez decide de garder vos armes actuelles.\n");
    }
    else
    {
        printf("Choix invalide. Aucune arme n'a ete remplacee.\n");
    }
}

void changeIsEquippedToWeaponsPlayer(WeaponsPlayer *weapons, int numberWeapon)
{

    WeaponsPlayer* currentWeapon = weapons;
    int currentNumber = 1;
    // parcours les armes
    while (currentWeapon != NULL)
    {
        // si le numéro de l'arme selectionner = celui de l'arme :
        if (currentNumber == numberWeapon)
        {
            // on l'équipe.
            currentWeapon->weapon.isEquipped = 1;
            printf("Vous avez equipe '%s'\n", currentWeapon->weapon.name);
        }
        else
        {
            currentWeapon->weapon.isEquipped = 0;
        }
        currentWeapon = currentWeapon->next;
        currentNumber++;
    }
}

//PARTIE RANDOM NAME WEAPON

// ah oui y'en as bcp
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

char* generateRandomWeaponName()
{
    int randomIndex = rand() % (sizeof(weaponNames) / sizeof(weaponNames[0]));
    return weaponNames[randomIndex];
}