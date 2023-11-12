#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Player.h"
#include "sqlite3.h"
#include "Armor.h"

//PARTIE ARMOR

Armor* createArmor(int defense)
{
    // on alloue la nouvelle armure.
    Armor *newArmor = malloc(sizeof(Armor));

    // nom random
    char *randomName = generateRandomArmorName();
    newArmor->name = randomName;
    newArmor->defense = defense;
    newArmor->isEquipped = 0;
    newArmor->next = NULL;
    return newArmor;
}

Armor* createArmorSave(int defense, const char* name)
{
    // on alloue la nouvelle armure.
    Armor *newArmor = malloc(sizeof(Armor));

    newArmor->name = strdup(name);

    newArmor->defense = defense;
    newArmor->isEquipped = 0;
    newArmor->next = NULL;
    return newArmor;
}


// supp
void deleteArmor(Armor *armor)
{
    free(armor);
}

void displayArmor(Armor* armor)
{
    printf("Armor - %s (defense: %d)", armor->name, armor->defense);
    if(armor->isEquipped == 1)
    {
        printf(" (Equipped)");
    }
    printf("\n");
}

//PARTIE LISTE CHAINEES DE ARMORS
// meme comment que pour l'arme.
int* addArmorsPlayer(st_player * player, Armor* New_Armor)
{
    printf("DEBUT FUNC ADD ARMOR PLAYER \n");

    int count = countArmorsPlayer(player);
    printf("nb armures : %d\n", count);
    if (count >= 5)
    {
        printf("Inventaire d'armures plein !\n");
        return player->armors;
    }
//  findEnd = la nouvelle arme.
    Armor * armor = (Armor *)player->armors;
    if (armor == NULL)
    {
        player->armors = (int *)New_Armor;
        printf("Insert armor (1st) \n");
    }
    else
    {
        // tant que liste pas finie.
        while (armor->next != NULL)
        {
            // nouvelle arme(s) = sa suivante.
            armor = (Armor *)armor->next;
            printf("next .. \n");
        }
        // Sa suivante = la nouvelle arme.
        armor->next = (int *)New_Armor;
        printf("Insert armor (2nd) \n");
    }

    printf("FIN FUNC :\n\n");
    // Retourner la nouvelle tête de la liste.
    return player->armors;
}

void deleteArmorsPlayer(st_player * player)
{
    Armor * armors = (Armor * )player->armors;
    // parcours toutes les armes.
    while (armors != NULL)
    {
        Armor *tmp = armors;
        // sa suivante.
        armors = (Armor *)armors->next;
        // et les supprimes
        deleteArmor(tmp);
    }
}

int countArmorsPlayer(st_player * player)
{
    int count = 0;
    int * next = player->armors;

    while (next != NULL)
    {
        count++;
        next = ((Armor *)next)->next;
    }
    return count;
}

Armor* getArmorNumberToArmorsPlayer(st_player * player, int number)
{
    Armor * currentArmor = (Armor* )player->armors;
    int currentNumber = 1;

    while (currentArmor != NULL)
    {
        if (currentNumber == number) {
            return currentArmor;
        }

        currentArmor = (Armor *)currentArmor->next;
        currentNumber++;
    }
    return NULL;
}

void displayArmorsPlayer(int* armors)
{
    printf("----------\n");
    printf("Armures :\n");
    Armor *tmp = (Armor *)armors;
    int value = 1;
    while (tmp != NULL)
    {
        printf("%d - ", value);
        displayArmor(tmp);
        tmp = (Armor *)tmp->next;
        value++;
    }
    printf("----------\n");
}

void swapArmorsPlayer(st_player *p_player, Armor newArmor)
{
    displayArmorsPlayer(p_player->armors);

    printf("Selectionnez le numero de l'armure que vous souhaitez remplacer : ");
    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= countArmorsPlayer(p_player))
    {
        Armor *armorToSwap = getArmorNumberToArmorsPlayer(p_player, choice);

        if (armorToSwap->isEquipped)
        {
            armorToSwap->defense = newArmor.defense;
            armorToSwap->name = newArmor.name;
            armorToSwap->isEquipped = newArmor.isEquipped;
        }
        printf("Vous avez remplace l'armure selectionnee par :\n");
        displayArmor(&newArmor);
    }
    else if (choice == 9)
    {
        printf("Vous avez decide de garder vos armures actuelles.\n");
    }
    else
    {
        printf("Choix invalide. Aucune armure n'a ete remplacee.\n");
    }
}

void changeIsEquippedToArmorsPlayer(st_player* p_player, int numberArmor)
{
    Armor * currentArmor = (Armor *)p_player->armors;
    int currentNumber = 1;

    while (currentArmor != NULL)
    {
        if (currentNumber == numberArmor)
        {
            currentArmor->isEquipped = 1;
            printf("Vous avez equipe '%s'\n", currentArmor->name);
        } else
        {
            currentArmor->isEquipped = 0;
        }
        currentArmor = (Armor *)currentArmor->next;
        currentNumber++;
    }
}

char* armorNames[] = {
        "Armure de Cuir",
        "Armure de Mailles",
        "Armure de Fer",
        "Armure de Bois",
        "Armure des Abysses",
        "Armure du Crepuscule",
        "Armure de la Nuit",
        "Armure de l Aube",
        "Armure de Pierre",
        "Armure de Bronze",
        "Armure de Marbre",
        "Armure d Acier",
        "Armure du Desert",
        "Armure de Glace",
        "Armure de Feu",
        "Armure de Vent",
        "Armure de l'Ocean",
        "Armure du Ciel",
        "Armure de la Jungle",
        "Armure des Montagnes",
        "Armure des Sables Mouvants",
        "Armure des Plaines",
        "Armure de la Toundra",
        "Armure de la Foret",
        "Armure de la Caverne",
        "Armure du Volcan",
        "Armure du Canyon",
        "Armure de l Eclair",
        "Armure de la Tempete",
        "Armure de la Foudre",
        "Armure de l Orage",
        "Armure de l Etoile",
        "Armure du Cosmos",
        "Armure du Vide",
        "Armure des Elements",
        "Armure de la Terre",
        "Armure de l Eau",
        "Armure du Feu Sacre",
        "Armure de la Lave",
        "Armure de l Ancien",
        "Armure du Gardien",
        "Armure du Protecteur",
        "Armure du Destin",
        "Armure de l Espoir",
        "Armure de la Sagesse",
        "Armure de la Discretion",
        "Armure du Courage",
        "Armure de la Resilience",
        "Armure de l Honneur",
        "Armure de la Victoire"
};

char* generateRandomArmorName() {
    int randomIndex = rand() % (sizeof(armorNames) / sizeof(armorNames[0]));
    return armorNames[randomIndex];
}
