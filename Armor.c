#include "Armor.h"
#include "Display.h"
//PARTIE ARMOR

Armor* createArmor(int level) {
    Armor *newArmor = malloc(sizeof(Armor));

    char *randomName = generateRandomArmorName();
    newArmor->name = randomName;

    newArmor->defense = level;
    newArmor->isEquipped = 0;
    return newArmor;
}

void deleteArmor(Armor *armor) {
    free(armor);
}

void displayArmor(Armor* armor) {
    printf("Armor - %s (defense: %d)", armor->name, armor->defense);
    if(armor->isEquipped == 1){
        printf(" (Equipped)");
    }
    printf("\n");
}

//PARTIE LISTE CHAINEES DE ARMORS

ArmorsPlayer* addArmorsPlayer(ArmorsPlayer *armors, Armor armor) {
    int count = countArmorsPlayer(armors);
    if (count == 5) {
        swapArmorsPlayer(armors, armor);
        return armors;
    }

    ArmorsPlayer *newArmor = malloc(sizeof(ArmorsPlayer));
    newArmor->armor = armor;
    newArmor->next = NULL;

    //debug
    if (armors == NULL){
        printf("armors == null\n");
        return newArmor;
    }

    ArmorsPlayer *findEnd = armors;
    while (findEnd->next != NULL){
        findEnd = findEnd->next;
    }

    findEnd->next = newArmor;
    return armors;
}

void deleteArmorsPlayer(ArmorsPlayer *armors) {
    while (armors != NULL){
        ArmorsPlayer *tmp = armors;
        armors = armors->next;
        deleteArmor((&tmp->armor));
        free(tmp);
        printf("debug :: suppression armure");
    }
}

int countArmorsPlayer(ArmorsPlayer *armors) {
    int count = 0;
    ArmorsPlayer *tmp = armors;

    while (tmp != NULL){
        count++;
        tmp = tmp->next;
    }
    return count;
}

ArmorsPlayer* getArmorNumberToArmorsPlayer(ArmorsPlayer *armors, int number) {
    ArmorsPlayer * currentArmor = armors;
    int currentNumber = 1;

    while (currentArmor != NULL) {
        if (currentNumber == number) {
            return currentArmor;
        }

        currentArmor = currentArmor->next;
        currentNumber++;
    }
    return NULL;
}

void displayArmorsPlayer(ArmorsPlayer *armors) {
    printf("----------\n");
    printf("Armures :\n");
    ArmorsPlayer *tmp = armors;
    int value = 1;
    while (tmp != NULL){
        printf("%d - ", value);
        displayArmor(&tmp->armor);
        tmp = tmp->next;
        value++;
    }
    printf("----------\n");
}

void swapArmorsPlayer(ArmorsPlayer *armors, Armor newArmor) {
    displayArmorsPlayer(armors);

    printf("Selectionnez le numero de l'armure que vous souhaitez remplacer : ");
    int choice;
    scanf("%d", &choice);
    clearScreen();

    if (choice >= 1 && choice <= countArmorsPlayer(armors)) {
        ArmorsPlayer *armorToSwap = getArmorNumberToArmorsPlayer(armors, choice);

        if (armorToSwap->armor.isEquipped) {
            armorToSwap->armor.isEquipped = 0;
            newArmor.isEquipped = 1;
        }

        armorToSwap->armor = newArmor;
        printf("Vous avez remplace l'armure selectionnee par :\n");
        displayArmor(&newArmor);
    } else if (choice == 9){
        printf("Vous avez decide de garder vos armures actuelles.\n");
    } else {
        printf("Choix invalide. Aucune armure n'a ete remplacee.\n");
    }
}

void changeIsEquippedToArmorsPlayer(ArmorsPlayer *armors, int numberArmor) {
    ArmorsPlayer * currentArmor = armors;
    int currentNumber = 1;


    while (currentArmor != NULL) {
        if (currentNumber == numberArmor) {
            currentArmor->armor.isEquipped = 1;
            printf("Vous avez equipe '%s'\n", currentArmor->armor.name);
        } else {
            currentArmor->armor.isEquipped = 0;
        }
        currentArmor = currentArmor->next;
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
        "Armure de l'Aube",
        "Armure de Pierre",
        "Armure de Bronze",
        "Armure de Marbre",
        "Armure d'Acier",
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
        "Armure de l'Eclair",
        "Armure de la Tempete",
        "Armure de la Foudre",
        "Armure de l'Orage",
        "Armure de l'Etoile",
        "Armure du Cosmos",
        "Armure du Vide",
        "Armure des Elements",
        "Armure de la Terre",
        "Armure de l'Eau",
        "Armure du Feu Sacre",
        "Armure de la Lave",
        "Armure de l'Ancien",
        "Armure du Gardien",
        "Armure du Protecteur",
        "Armure du Destin",
        "Armure de l'Espoir",
        "Armure de la Sagesse",
        "Armure de la Discretion",
        "Armure du Courage",
        "Armure de la Resilience",
        "Armure de l'Honneur",
        "Armure de la Victoire"
};

char* generateRandomArmorName() {
    int randomIndex = rand() % (sizeof(armorNames) / sizeof(armorNames[0]));
    return armorNames[randomIndex];
}
