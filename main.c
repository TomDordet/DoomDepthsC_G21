#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "Monster.h"
#include "level.h"
#include "sqlite3.h"
#include "Sort.h"
#include "Display.h"



// PROTOTYPE FONCTIONS (pas issue de .h donc on les protos ici)
// FIGHT
st_monsters *fight_player_round(st_player *p_player, st_monsters *p_first_monster, int nb_level); //proto de la func joueur attaque monstre
st_player *fight_monsters_round(st_player *p_player, st_monsters *p_first_monster); //proto de la func monstre(s) attaque joueur
st_monsters *sort_player_round (st_player *p_player, st_monsters *p_first_monster, Sort sort);


//HEAL
int healLife(st_player *p_player); //proto healLife
int healMana(st_player *p_player); //proto healMana


// INSERT DATA (SAVE)
int insertTheSAVE(sqlite3 *db, st_player* p_player, st_level* p_level);

// LOAD SAVE
int displayLatestSaves(void);

// --------------- BDD -----------------------------------

// create_table prend en param la db
int create_tables(sqlite3 *db)
{

    const char *create_joueur_table_sql =
            "CREATE TABLE JOUEUR ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "maxLife INTEGER, "
            "currentLife INTEGER, "
            "attack INTEGER, "
            "id_niveau INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (id_niveau) REFERENCES LEVEL(lvl_number), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_monstre_table_sql =
            "CREATE TABLE MONSTRE ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "number INTEGER, "
            "maxLife INTEGER, "
            "currentLife INTEGER, "
            "attack INTEGER, "
            "p_next INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (p_next) REFERENCES MONSTRE(id), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_level_table_sql =
            "CREATE TABLE LEVEL ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "lvl_number INTEGER, "
            "nb_monster INTEGER, "
            "p_monster INTEGER, "
            "id_sauvegarde INTEGER, "
            "FOREIGN KEY (p_monster) REFERENCES MONSTRE(id), "
            "FOREIGN KEY (id_sauvegarde) REFERENCES SAUVEGARDE(id) "
            ");";

    const char *create_sauvegarde_table_sql =
            "CREATE TABLE SAUVEGARDE ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "currentLevel INTEGER, "
            "date TEXT "
            ");";
    /*
    const char *create_level_monstre_table_sql =
            "CREATE TABLE LEVEL_MONSTER ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "id_niveau INTEGER, "
            "id_monstre INTEGER, "
            "FOREIGN KEY (id_niveau) REFERENCES LEVEL(lvl_number), "
            "FOREIGN KEY (id_monstre) REFERENCES MONSTRE(id)"
            ");";

    */
    /*
    int rc = 0;

    const char * drop_joueur_sql = "DROP TABLE JOUEUR;";
    const char * drop_monster_sql = "DROP TABLE MONSTRE;";
    const char * drop_level_sql = "DROP TABLE LEVEL;";
    const char * drop_sauvegarde_sql = "DROP TABLE SAUVEGARDE;";

    rc = sqlite3_exec(db, drop_joueur_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_monster_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_level_sql, 0, 0, 0);
    rc = sqlite3_exec(db, drop_sauvegarde_sql, 0, 0, 0);
    */


    int rc = 0;

    rc = sqlite3_exec(db, create_joueur_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_monstre_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_level_table_sql, 0, 0, 0);
    rc = sqlite3_exec(db, create_sauvegarde_table_sql, 0, 0, 0);

    return 0;
}

// Création / ouverture de la base de données
int init_bdd(void)
{
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("D:/Bilal/Code/DoomDepths 2/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Créez les tables
    rc = create_tables(db);
    if (rc != 0) {
        fprintf(stderr, "Erreur lors de la création des tables.\n");
    }
    sqlite3_close(db);
    return 0;
}

int insertData(st_player* p_player, st_level* p_level)
{
    fprintf(stderr, "Début de la fonction insertData\n");
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("D:/Bilal/Code/DoomDepths 2/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    insertTheSAVE(db, p_player, p_level);

    sqlite3_close(db);
    fprintf(stderr, "Fin de la fonction insertData\n");
    return 0;
}

// --------------- DISPLAY ENTRE LVL ---------------------

//-----------------PARTIE INVENTAIRE-----------------

void display_inventory(st_player* p_player){
    int option = 1;
    char saisie2[256];

    while (option != 4){
        printf("1. Armes\n");
        printf("2. Armures\n");
        printf("3. Sorts\n");
        printf("4. Retour\n");
        scanf(" %s", saisie2);
        clearScreen();
        barreAffichage(p_player);
        afficherHero(p_player);
        if (sscanf(saisie2, "%d", &option) != 1) {
            printf("Veuillez entrer un chiffre valide.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1: //Inventaire -> Armes
                displayWeaponsPlayer(p_player->weapons);
                int weaponOption = 1;
                char saisie3[256];

                while (weaponOption != 2) {
                    printf("1. Changer d'arme\n");
                    printf("2. Retour\n");
                    scanf(" %s", saisie3);
                    clearScreen();
                    barreAffichage(p_player);
                    afficherHero(p_player);
                    if (sscanf(saisie3, "%d", &weaponOption) != 1) {
                        printf("Veuillez entrer un chiffre valide.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    int equipWeapon = -1;
                    switch(weaponOption){
                        case 1: //Armes -> Changer d'arme
                            displayWeaponsPlayer(p_player->weapons);
                            printf("Quelle arme equiper?\n");
                            scanf("%d", &equipWeapon);
                            clearScreen();
                            barreAffichage(p_player);
                            afficherHero(p_player);
                            if (equipWeapon == -1){
                                break;
                            }
                            else if (equipWeapon > countWeaponsPlayer(p_player->weapons)){
                                printf("Erreur de saisie : aucune arme ne correspond au numero %d\n", equipWeapon);
                                break;
                            }
                            else {
                                changeIsEquippedToWeaponsPlayer(p_player->weapons, equipWeapon);
                                changeMinAndMaxAttackValues(p_player);
                            }
                        case 2: //Armes -> Retou
                            break;
                        default:
                            printf("Aucune option ne correspond a votre saisi. Veuillez reesayer\n");
                            break;
                    }
                }
                break;
            case 2: //Inventaire -> Armures
                displayArmorsPlayer(p_player->armors);
                int armorOption = 1;
                char saisie4[256];

                while (armorOption != 2) {
                    printf("1. Changer d'armure\n");
                    printf("2. Retour\n");
                    scanf(" %s", saisie4);

                    clearScreen();
                    barreAffichage(p_player);
                    afficherHero(p_player);


                    if (sscanf(saisie4, "%d", &armorOption) != 1) {

                        printf("Veuillez entrer un chiffre valide.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    int equipArmor = -1;
                    switch (armorOption) {
                        case 1: //Armures -> Changer d'armure
                            displayArmorsPlayer(p_player->armors);
                            printf("Quelle armure equiper?\n");
                            scanf("%d", &equipArmor);
                            if (equipArmor == -1) {
                                break;
                            } else if (equipArmor > countArmorsPlayer(p_player->armors)) {
                                printf("Erreur de saisie : aucune arme ne correspond au numero %d\n", equipArmor);
                                break;
                            } else {
                                changeIsEquippedToArmorsPlayer(p_player->armors, equipArmor);
                                changeDefenseValue(p_player);
                            }
                        case 2: //Armures -> Retour
                            break;
                        default:
                            printf("Aucune option ne correspond a votre saisi. Veuillez reesayer\n");
                            break;
                    }
                }
                break;
            case 3: //Inventaire -> Potions
                printf("----------\n");
                printf("Description des sorts :\n");
                for (enum Sorts sort = BOULEDEFEU; sort <= REGENERATIONMANA; sort++) {
                    afficherDescriptionSort(sort);
                }
                printf("----------\n");
                break;
            case 4: //Inventaire -> Retour
                break;

            default:
                printf("Aucune option ne correspond a votre saisi. Veuillez reesayer\n");
                break;
        }
    }
}
//-----------------FIN PARTIE INVENTAIRE-----------------

int first_menu(st_player *p_player)
{
    clearScreen();
    affichageMenu(p_player, get_lvl_monsters(get_lvl()));
    while(1)
    {
        int save_choice = 1;
        char saisie[256];

        //Récupérer les armes et armures potentiellement lootées par des monstres et les ajouter à l'inventaire du joueur.
        //une boucle qui ajoute toutes les armes lootées dans la liste chainées weapons du joueur par le biai de la fonction addWeaponsPlayer

        //une boucle qui ajoute toutes les armures lootées dans la liste chainées armors du joueur par le biai de la fonction addWeaponsPlayer

        //je souhaite que les différents tableaux/ou listes chainées qui servaient à stocker temporairement mes équipements soient vidées.

        printf("Quelques minutes de pause avant de reprendre... Que voulez vous faire ? \n");
        printf("1 - Inventaire\n");
        printf("2 - Sauvegarde\n");
        printf("3 - Continuer\n");
        printf("4 - Exit\n");

        scanf(" %s", saisie);

        if (sscanf(saisie, "%d", &save_choice) != 1) {
            printf("Veuillez entrer un chiffre valide.\n");
            while (getchar() != '\n');
            continue;
        }

        switch(save_choice){
            case 1: {
                clearScreen();
                affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                display_inventory(p_player);
                break;
            }
            case 2:
                return 2;
            case 3:
                clearScreen();
                return 3;
            case 4:
                return 4;
            default:
                printf("Aucune option ne correspond a votre saisi. Veuillez reesayer\n");
                break;
        }
    }
}

// ---- LA

//fin de game
int exit_game (st_player *p_player)
{
    //Libere tous les montres
    delete_all_level();
    //libere le joueur
    delete_player(p_player);
    return 0;
}

int game(int id_db)
{
    int choixMenu = 1;
    char saisie[256];
    printf("DEBUG --- %s ----id_db %d \n", __FUNCTION__ ,id_db);
    st_player *p_player = create_player(id_db); // on créer le joueur.
    init_level(id_db); // init le lvl (qui init la créa des mstr)
    clearScreen();
    while (1)
    {
        clearScreen();
        barreAffichage(p_player);
        afficherHero();
        printf("--- LEVEL %d ----\n", get_lvl());
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.heal \n");
        printf("5.Inventaire\n");
        printf("9.sortir \n");
        printf("Selectionner une des options: \n");
        scanf(" %s", saisie);

        if (sscanf(saisie, "%d", &choixMenu) != 1) {
            printf("Veuillez entrer un chiffre valide.\n");
            // Effacer le tampon d'entrée pour éviter les boucles infinies en cas de saisie invalide
            while (getchar() != '\n');
            continue;  // Reprendre la boucle
        }

        switch (choixMenu)
        {
            case 1: {
                clearScreen();
                barreAffichage(p_player);
                afficherHero();
                display_player(p_player); // affiche les stats du joueur.
                sleep(5);
                break;
            }
            case 2:
            {
                clearScreen();
                affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                printf("-- Level %d -- \n",get_lvl()); // affiche le LVL actuel
                display_monsters(get_lvl_monsters(get_lvl())); // affiche les monstres.
                sleep(5);
                break;
            }
            case 3:
            {
                clearScreen();
                int choixAttaque = 1;
                char saisie0[256];

                do{

                    affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                    printf("attaque %d\n", p_player->attack);

                    

                    printf("defense %d\n", p_player->defense);
                    printf("1. Attaquer\n");
                    printf("2. Lancer un sort\n");
                    printf("0. Retour\n");
                    printf("Votre selection: \n");

                    scanf(" %s", saisie0);
                    clearScreen();
                    affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                    if (sscanf(saisie0, "%d", &choixAttaque) != 1) {
                        printf("Veuillez entrer un chiffre valide.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    switch (choixAttaque) {
                        case 1: {
                            /*Game*/
                            // Si le retour de fight_player_round == NULL, alors monstres morts / désallouer (voir ce que la fonction retourne à sa définition, pour comprendre).
                            st_monsters * p_fight = fight_player_round(p_player, get_lvl_monsters(get_lvl()), get_lvl());
                            if (p_fight == NULL) {
                                int f_menu;

                                // on remet à NULL les monstres dans le lvl, pck ils sont morts.
                                set_lvl_monsters(NULL, get_lvl());
                                printf("Tout les monstres sont morts !\n");
                                f_menu = first_menu(p_player);
                                if (f_menu == 2) {
                                    insertData(p_player, g_st_level);
                                } else if (f_menu == 3) {//next game, next level
                                    //next_game();
                                    next_level();
                                } else if (f_menu == 4)//Exit
                                {
                                    return exit_game(p_player);
                                }
                                break;
                            } else if (p_fight != get_lvl_monsters(get_lvl())) {
                                set_lvl_monsters(p_fight, get_lvl());
                            } else {
                                //pNew == get_lvl_monsters(get_lvl()
                                //donc on fait rien
                            }

                            p_player = fight_monsters_round(p_player, get_lvl_monsters(
                                    get_lvl())); // déclenche l'attaque du monstre.
                            if (NULL == p_player) // si joueur = NULL (donc mort...)
                            {
                                printf("Player is dead (has been deleted) .. Game over !!!!! \n ");
                                /*Libere tous les montres*/
                                delete_all_level();
                                gameOver();
                                sleep(5);
                                clearScreen();
                                return 0; // ?
                            }
                            break;
                        }

                        case 2:
                        {
                            int choixSort = 1;
                            char saisie1[256];
                            clearScreen();
                            affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                            printf("1. Boule de Feu\n");
                            printf("2. Eclair Fulgurant\n");
                            printf("3. Decuplement\n");
                            printf("4. Mur de glace\n");
                            printf("5. Bouclier de lumiere\n");
                            printf("6. Regeneration de vie\n");
                            printf("7. Regeneration de mana\n");
                            printf("0. Retour\n");
                            printf("Selectionner une option: \n");

                            scanf(" %s", saisie1);
                            clearScreen();
                            affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                            if (sscanf(saisie1, "%d", &choixSort) != 1) {
                                printf("Veuillez entrer un chiffre valide.\n");
                                while (getchar() != '\n');
                                continue;
                            }

                            Sort mon_sort;
                            st_monsters *p_fight = NULL;


                            switch (choixSort) {
                                case 1:
                                {
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 50+1;
                                    mon_sort.sort = BOULEDEFEU;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = OFFENSIVE;
                                    if(p_player->currentMana < mon_sort.resources){
                                        printf("Pas assez de mana pour ce sort\n");
                                        break;
                                    }else{
                                        p_fight = sort_player_round(p_player, get_lvl_monsters(get_lvl()), mon_sort);
                                    }

                                    if (p_fight == NULL) {
                                        int f_menu;

                                        set_lvl_monsters(NULL, get_lvl());

                                        printf("Tout les monstres sont morts !\n");
                                        p_player->attack = 10;

                                        

                                        p_player->defense = 10;
                                        f_menu = first_menu(p_player);
                                        if (f_menu == 2) {
                                            insertData(p_player, g_st_level);
                                        } else if (f_menu == 3) {
                                            next_level();
                                        } else if (f_menu == 4)//Exit
                                        {
                                            return exit_game(p_player);
                                        }
                                        break;
                                    } else if (p_fight != get_lvl_monsters(get_lvl())) {
                                        set_lvl_monsters(p_fight, get_lvl());
                                    } else {
                                        //pNew == get_lvl_monsters(get_lvl()
                                        //donc on fait rien
                                    }

                                    p_player = fight_monsters_round(p_player, get_lvl_monsters(
                                            get_lvl())); // déclenche l'attaque du monstre.
                                    if (NULL == p_player) // si joueur = NULL (donc mort...)
                                    {
                                        printf("Player is dead (has been deleted) .. Game over !!!!! \n ");
                                        /*Libere tous les montres*/
                                        delete_all_level();
                                        gameOver();
                                        sleep(5);
                                        clearScreen();
                                        return 0; // ?
                                    }
                                    break;

                                }
                                case 2:
                                {
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 50+1;
                                    mon_sort.sort = ECLAIRFULGURANT;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = OFFENSIVE;
                                    if(p_player->currentMana < mon_sort.resources){
                                        printf("Pas assez de mana pour ce sort\n");
                                        break;
                                    }else{
                                        p_fight = sort_player_round(p_player, get_lvl_monsters(get_lvl()), mon_sort);
                                    }

                                    if (p_fight == NULL) {
                                        int f_menu;

                                        set_lvl_monsters(NULL, get_lvl());
                                        printf("Tout les monstres sont morts !\n");
                                        p_player->minAttack = 10;
                                        p_player->defense = 10;
                                        f_menu = first_menu(p_player);
                                        if (f_menu == 2) {
                                            insertData(p_player, g_st_level);
                                        } else if (f_menu == 3) {
                                            next_level();
                                        } else if (f_menu == 4)//Exit
                                        {
                                            return exit_game(p_player);
                                        }
                                        break;
                                    } else if (p_fight != get_lvl_monsters(get_lvl())) {
                                        set_lvl_monsters(p_fight, get_lvl());
                                    } else {
                                        //pNew == get_lvl_monsters(get_lvl()
                                        //donc on fait rien
                                    }

                                    p_player = fight_monsters_round(p_player, get_lvl_monsters(
                                            get_lvl())); // déclenche l'attaque du monstre.
                                    if (NULL == p_player) // si joueur = NULL (donc mort...)
                                    {
                                        printf("Player is dead (has been deleted) .. Game over !!!!! \n ");
                                        /*Libere tous les montres*/
                                        delete_all_level();
                                        gameOver();
                                        sleep(5);
                                        clearScreen();
                                        return 0; // ?
                                    }

                                    break;
                                }
                                case 3:
                                {

                                    int damage = rand() % 5+2;
                                    int ressources = rand() % 30+1;
                                    mon_sort.sort = DECUPLEMENT;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = OFFENSIVE;
                                    sort(p_player, p_fight, mon_sort);
                                    break;
                                }
                                case 4:
                                {//pk quand je clique sur 4 il sort du programme, parce que p_monster est à null
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 30+1;
                                    mon_sort.sort = MURDECLACE;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = DEFENSIVE;
                                    sort(p_player, p_fight, mon_sort);


                                    break;
                                }
                                case 5:
                                {
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 50+1;
                                    mon_sort.sort = BOUCLIERDELUMIERE;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = DEFENSIVE;
                                    sort(p_player, p_fight, mon_sort);


                                    break;
                                }
                                case 6:
                                {
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 50+1;
                                    mon_sort.sort = REGENERATIONVIE;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = LIFEHEAL;
                                    sort(p_player, p_fight, mon_sort);
                                    break;
                                }
                                case 7:
                                {
                                    int damage = rand() % 50+1;
                                    int ressources = rand() % 50+1;
                                    mon_sort.sort = REGENERATIONMANA;
                                    mon_sort.damage = damage;
                                    mon_sort.resources = ressources;
                                    mon_sort.type = MANAHEAL;
                                    sort(p_player, p_fight, mon_sort);
                                    break;
                                }
                                case 0:
                                    break;

                                default:
                                    printf("Choix invalide. Veuillez reessayer.\n");
                                    break;

                            }
                            clearScreen();
                            printf("Vie: %d / %d \n", p_player->currentLife, p_player->maxLife);
                            printf("Mana: %d / %d \n", p_player->currentMana, p_player->maxMana);

                        }

                        case 0:
                            break;

                        default:
                            printf("Choix invalide. Veuillez réessayer.\n");
                            break;

                    }
                } while (choixAttaque != 0);
                break;

            }

            case 4: {
                clearScreen();
                affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                int choixPotions = 1;
                char saisie0[256];

                do {
                    clearScreen();
                    affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                    printf("1. Potions de vie\n");
                    printf("2. Potions de Mana\n");
                    printf("0. Retour\n");
                    printf("Votre selection: \n");

                    scanf(" %s", saisie0);
                    if (sscanf(saisie0, "%d", &choixPotions) != 1) {
                        printf("Veuillez entrer un chiffre valide.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    switch (choixPotions) {
                        case 1:
                            healLife(p_player);
                            break;

                        case 2:
                            healMana(p_player);
                            break;

                        case 0:
                            break;
                    }
                } while (choixPotions != 0);
                break;
            }
            case 5: {
                clearScreen();
                affichageMenu(p_player, get_lvl_monsters(get_lvl()));
                display_inventory(p_player);
                break;
            }
            case 9:
            {
                clearScreen();
                int var;
                printf("Tout sera supprimer, continuer ? (1/0)\n");
                scanf("%d", &var);
                if (var == 1)
                {
                    return exit_game (p_player);
                }
                break;
            }
            default:
                printf("Aucune option ne correspond a votre saisi, veuillez reesayer\n");
                break;
        }
    }


    //system("PAUSE");
    return 0;
}
// ---------------- WELCOME ----------------

int welcome(void)
{
    int choixMenu = 0;
    int id_db = 0;
    char saisie[256];

    while (choixMenu != 3)
    {
        clearScreen();
        doomDepths();
        printf("\n\n");
        printf("1 - Start Game \n");
        printf("2 - Load save \n");
        printf("3 - Exit\n");
        printf("Selectionner une des options: ");
        scanf(" %s", saisie);

        if (sscanf(saisie, "%d", &choixMenu) != 1) {
            printf("Veuillez entrer un chiffre valide.\n\n");
            // Effacer le tampon d'entrée pour éviter les boucles infinies en cas de saisie invalide
            while (getchar() != '\n');
            continue;  // Reprendre la boucle
        }


        switch (choixMenu) {
            case 1: {
                clearScreen();
                game(id_db);
                break;
            }
            case 2:
            {
                clearScreen();
                id_db = displayLatestSaves();
                printf("ID DB is %d\n", id_db);
                break;
            }
            case 3: {
                clearScreen();
                break;
            }
            default:
                printf("Aucune option ne correspond a votre saisi, veuillez reesayer\n");
                break;
        }
    }
    return 0;
}

int main(void)
{
    init_bdd();
    welcome();
}