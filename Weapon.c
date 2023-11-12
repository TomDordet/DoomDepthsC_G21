#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "sqlite3.h"


//PARTIE WEAPON
#include "Weapon.h"
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
    newWeapon->next = NULL;
    return newWeapon;
}

Weapon *createWeaponSave(int nb_attack_per_round, int minDamage, int maxDamage, const char* name)
{
    printf("ALLOC ARME OK\n");
    // alloc de l'arme
    Weapon *newWeapon = malloc(sizeof(Weapon));

    newWeapon->name = strdup(name);

    printf("NAME : %s\n", newWeapon->name);
    newWeapon->nb_attack_per_round = nb_attack_per_round;

    printf("NB ATTACK : %d\n", newWeapon->nb_attack_per_round);
    newWeapon->minDamage = minDamage;
    newWeapon->maxDamage = maxDamage;
    newWeapon->isEquipped = 0;
    newWeapon->next = NULL;

    printf("OK\n\n");
    return newWeapon;
}

// free l'arme
void deleteWeapon(Weapon * weapon)
{
    printf("debug :: suppression weapons %p \n",weapon);
    free(weapon);
}


void displayWeapon(Weapon * weapon)
{
    // affiche l'arme.
    printf("Weapon - %s (damage: %d-%d, attacks per turn: %d)",
           weapon->name,
           weapon->minDamage,
           weapon->maxDamage,
           weapon->nb_attack_per_round);

    // affiche en + si elle est équipé ou non.
    if(weapon->isEquipped == 1)
    {
        printf(" (Equipped)");
    }
    printf("\n");
}

int* addWeaponsPlayer(st_player *player, Weapon* New_Weapon)
{
    printf("debut dunc addWeaponPlayer\n");

    // nb d'armes du joueur.
    int count = countWeaponsPlayer(player);
    printf("nb armes : %d\n", count);
    if (count >= 5)
    {
        // si joueur a déjà plus de 5 armes :
        printf("Inventaire d'armes plein !\n");
        //free(New_Weapon); // Atention dangereux de liberer la memoire ici, il faudrait retourner un status et le tester a l'appelant.
        return player->weapons;
    }

    //  findEnd = la nouvelle arme.
    Weapon * weapon = (Weapon *)player->weapons;
    if (weapon == NULL)
    {
        player->weapons = (int *)New_Weapon;
        printf("Insert weapons (1st) \n");
    }
    else
    {
        // tant que liste pas finie.
        while (weapon->next != NULL)
        {
            // nouvelle arme(s) = sa suivante.
            weapon = (Weapon *)weapon->next;
            printf("next .. \n");
        }
        // Sa suivante = la nouvelle arme.
        weapon->next = (int *)New_Weapon;
        printf("Insert weapons (2nd) \n");
    }

    printf("FIN FUNC :\n\n");
    // Retourner la nouvelle tête de la liste.
    return player->weapons;
}

// free les armes.
void deleteWeaponsPlayer(st_player * p_player)
{
    Weapon * weapons = (Weapon * )p_player->weapons;
    // parcous toutes les armes.
    while (weapons != NULL)
    {
        Weapon *tmp = weapons;
        // sa suivante.
        weapons = (Weapon *)weapons->next;
        // et les supprimes
        deleteWeapon(tmp);

    }
}

// compte les armes que le joueur possède.
int countWeaponsPlayer(st_player * player)
{
    int count = 0;
    int * next = player->weapons;

    //parcours les armes.
    while (next != NULL)
    {
        count++;
        printf("%s next = %p , count %d \n",__FUNCTION__ ,next,count);
        //arme suivante.
        next = ((Weapon *)next)->next;
    }
    return count;
}

Weapon* getWeaponNumberToWeaponsPlayer(st_player * player, int number)
{

    Weapon* currentWeapon = (Weapon* )player->weapons;
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
        currentWeapon = (Weapon* )currentWeapon->next;
        currentNumber++;
    }
    return NULL;
}
#include "Weapon.h"
// liste les armes du joueur.
void displayWeaponsPlayer(int * weapons)
{
    printf("----------\n");
    printf("Armes :\n");
    Weapon *tmp = (Weapon*)weapons;
    int value = 1;
    while (tmp != NULL)
    {
        printf("%d - ", value);
        displayWeapon(tmp);
        tmp = (Weapon*)tmp->next;
        value++;
    }
    printf("----------\n");
}


void swapWeaponsPlayer(st_player *p_player, Weapon newWeapon)
{
    // affiche les armes du joueur.
    displayWeaponsPlayer(p_player->weapons);

    printf("Selectionnez le numero de l'arme que vous souhaitez remplacer : ");
    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= countWeaponsPlayer(p_player))
    {
        // parcours les armes du joueur, renvoie l'arme que l'on souhaite remplacer.
        Weapon *weaponToSwap = getWeaponNumberToWeaponsPlayer(p_player, choice);

        // si l'arme choisis est équipé, on l'équipe pu..
        if (weaponToSwap->isEquipped)
        {
            weaponToSwap->isEquipped = newWeapon.isEquipped;
            weaponToSwap->name = newWeapon.name;
            weaponToSwap->minDamage = newWeapon.minDamage;
            weaponToSwap->maxDamage = newWeapon.maxDamage;
            weaponToSwap->nb_attack_per_round = newWeapon.nb_attack_per_round;
        }
        // on remplace l'ancienne arme par la nouvelle dans l'inventaire du player.
        printf("Vous avez remplace l'arme selectionnee par :\n");
        displayWeapon(weaponToSwap);
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

void changeIsEquippedToWeaponsPlayer(st_player * p_player, int numberWeapon)
{

    Weapon* currentWeapon = (Weapon* )p_player->weapons;
    int currentNumber = 1;
    // parcours les armes
    while (currentWeapon != NULL)
    {
        // si le numéro de l'arme selectionner = celui de l'arme :
        if (currentNumber == numberWeapon)
        {
            // on l'équipe.
            currentWeapon->isEquipped = 1;
            printf("Vous avez equipe '%s'\n", currentWeapon->name);
        }
        else
        {
            currentWeapon->isEquipped = 0;
        }
        currentWeapon = (Weapon* )currentWeapon->next;
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
        "Katana de l Aube",
        "Poignard de l Empereur",
        "Baton de Sagesse",
        "Hache de Lave",
        "Marteau de Givre",
        "Fleau de la Nuit",
        "Dague des Abysses",
        "Epee de l Aigle",
        "Arc de la Lune",
        "Lance de l Etoile Filante",
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
        "Hache de l Inferno",
        "Marteau de la Colere",
        "Fleau de la Pestilence",
        "Dague du Bandit",
        "Epee de la Vengeance",
        "Arc de la Discorde",
        "Lance de la Ruine",
        "Katana de l Honneur",
        "Couteau de l Assassin",
        "Baton de l Illumination",
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