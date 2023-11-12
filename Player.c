#include "Player.h"

// fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
st_player * create_player (int id_db)
{
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.

    //Création d'une arme et d'une armure par défaut
    Weapon *startWeapon = createWeapon(1, 2, 5);
    startWeapon->isEquipped = 1;
    startWeapon->name = "Baton en bois";
    printf("%s\n",startWeapon->name);


    Armor *startArmor = createArmor(1);
    startArmor->isEquipped = 1;
    startArmor->name = "Tenue simple";
    printf("%s\n",startArmor->name);

    if (id_db == 0)
    {
        //default value, Hardcoded value.
        p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
        p_player->currentLife = p_player->maxLife;
        p_player->maxMana = 80;
        p_player->currentMana = p_player->maxMana;
        p_player->gold = 0;
        p_player->minAttack = startWeapon->minDamage;
        p_player->maxAttack = startWeapon->maxDamage;
        p_player->defense = startArmor->defense;
        p_player->weapons = NULL;
        p_player->armors = NULL;
    }
    else
    {
        //get db (id_db)
        p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
        p_player->currentLife = p_player->maxLife;
        p_player->maxMana = 80;
        p_player->currentMana = p_player->maxMana;
        p_player->gold = 0;
        p_player->minAttack = startWeapon->minDamage;
        p_player->maxAttack = startWeapon->maxDamage;
        p_player->defense = startArmor->defense;
        p_player->weapons = NULL;
        p_player->armors = NULL;

    }

    //Ajout des équipements par défaut dans la liste chainée
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *startWeapon);
    p_player->armors = addArmorsPlayer(p_player->armors, *startArmor);

    //PARTIE TEST WEAPONS
    Weapon *weapon2 = createWeapon(1, 2, 5);
    printf("%s\n",weapon2->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon2);
    Weapon *weapon3 = createWeapon(3, 6, 10);
    printf("%s\n",weapon3->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon3);
    Weapon *weapon4 = createWeapon(2, 20, 22);
    printf("%s\n",weapon4->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon4);
    Weapon *weapon5 = createWeapon(3, 7, 15);
    printf("%s\n",weapon5->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon5);

    printf("%d\n", countWeaponsPlayer(p_player->weapons));

    WeaponsPlayer *weaponsPlayer = getWeaponNumberToWeaponsPlayer(p_player->weapons, 4);
    printf("Nom : %s\n", weaponsPlayer->weapon.name);

    Weapon *weapon6 = createWeapon(2, 3, 14);
    printf("%s\n",weapon6->name);
    p_player->weapons = addWeaponsPlayer(p_player->weapons, *weapon6);
    //FIN PARTIE TEST ARMORS

  
    //PARTIE TEST WEAPONS
    Armor *armor2 = createArmor(2);
    printf("%s\n",armor2->name);
    p_player->armors = addArmorsPlayer(p_player->armors, *armor2);
    Armor *armor3 = createArmor(3);
    printf("%s\n",armor3->name);
    p_player->armors = addArmorsPlayer(p_player->armors, *armor3);
    Armor *armor4 = createArmor(4);
    printf("%s\n",armor4->name);
    p_player->armors = addArmorsPlayer(p_player->armors, *armor4);
    Armor *armor5 = createArmor(5);
    printf("%s\n",armor5->name);
    p_player->armors = addArmorsPlayer(p_player->armors, *armor5);

    printf("%d\n", countArmorsPlayer(p_player->armors));

    ArmorsPlayer *armorsPlayer = getArmorNumberToArmorsPlayer(p_player->armors, 4);
    printf("Nom : %s\n", armorsPlayer->armor.name);

    Armor *armor6 = createArmor(6);
    printf("%s\n",armor6->name);
    p_player->armors = addArmorsPlayer(p_player->armors, *armor6);

    //FIN PARTIE TEST ARMORS

    return p_player;
}


st_player * delete_player (st_player *p_player) //Supprime le joueur.
{
    //débug de vérification du free de l'ensemble des armes du joueur
    //je le fais ici car les armes ont besoin d'être free seulement lorsque le joueur meurt.
    deleteWeaponsPlayer(p_player->weapons);
    deleteArmorsPlayer(p_player->armors);

    printf("DEBUG :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.
    free (p_player);
    return NULL;
}

int display_player (st_player *p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Mana : %d / %d point de Mana. \n",p_player->currentMana, p_player->maxMana);
    printf("Or: %d\n", p_player->gold);
    //Affiche l'arme équipée
    WeaponsPlayer *tmp = p_player->weapons;
    while (tmp != NULL) {
        if (tmp->weapon.isEquipped == 1) {
            printf("Arme equipee : %s (damage: %d-%d)\n", tmp->weapon.name, p_player->minAttack, p_player->maxAttack);
            break;
        }
        tmp = tmp->next;
    }
    //Affiche l'armure équipée
    ArmorsPlayer *tmp2 = p_player->armors;
    while (tmp2 != NULL) {
        if (tmp2->armor.isEquipped == 1) {
            printf("Armure equipee : %s (defense: %d)\n", tmp2->armor.name, p_player->defense);
            break;
        }
        tmp2 = tmp2->next;
    }
    return 0;
}

void changeMinAndMaxAttackValues(st_player *p_player) {
    WeaponsPlayer *tmp = p_player->weapons;
    while (tmp != NULL) {
        if (tmp->weapon.isEquipped == 1) {
            p_player->maxAttack = tmp->weapon.maxDamage;
            p_player->minAttack = tmp->weapon.minDamage;
            break;
        }
        tmp = tmp->next;
    }
}

void changeDefenseValue(st_player *p_player){
    ArmorsPlayer *tmp = p_player->armors;
    while (tmp != NULL) {
        if (tmp->armor.isEquipped == 1) {
            p_player->defense = tmp->armor.defense;
            break;
        }
        tmp = tmp->next;
    }
}