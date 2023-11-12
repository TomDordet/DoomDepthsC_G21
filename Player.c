#include "Player.h"

// fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
st_player * create_player (int id_db)
{
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.

    //Création d'une arme et d'une armure par défaut
    Weapon *startWeapon = createWeapon(1);
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