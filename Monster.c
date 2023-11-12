#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Monster.h"
#include "sqlite3.h"

static int g_number_monster = 1; //

void reset_monster_number (void) // func pour reset le nb de monstres. (pour que quand on les supp, puis en recréer d'autres, le "compteur" de mstr" reparte de 1.
{
    g_number_monster = 1;
}

static void init_stats_monster (st_monsters * p_montser, int level_number)
{
    //default value, Hardcoded value.
    p_montser->number = g_number_monster++;
    //printf("DEBUG :: init_stats_monster func begin for monster %d\n", p_montser->number);
    p_montser->attack = 5 * level_number;
    p_montser->maxLife = ((rand() % (10)) * level_number);
    if (p_montser->maxLife == 0)
        p_montser->maxLife = 1; // on s'assure que la vie du monstre soit au moins de 1.
    p_montser->currentLife = p_montser->maxLife;
    p_montser->defense = rand() % 10; // a faire plus tard.
    //printf("DEBUG :: END FUNC for monster %d\n", p_montser->number);

}

static void init_stats_monster_save(st_monsters * p_montser, int save_id, int monster_number)
{
    // BDD
    int rc = 0;
    sqlite3 *db;
    rc = sqlite3_open("C:/Users/ethan/CLionProjects/DoomDepthsC_G21/DoomDepthsC_G21.db", &db);
    if (rc)
    {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
    }
    // SAME REQUETE MAIS QUI PREND EN PLUS LE NUMBER DU MONSTRE RECUP EN PARAMETRE
    const char *select_monster_sql = "SELECT maxLife, currentLife, attack, p_next FROM MONSTRE WHERE id_sauvegarde = ? AND number = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, select_monster_sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, save_id);
    sqlite3_bind_int(stmt, 2, monster_number);

    sqlite3_step(stmt);

    int monsterMaxLife = sqlite3_column_int(stmt, 0);
    int monsterCurrentLife = sqlite3_column_int(stmt, 1);
    int monsterAttack = sqlite3_column_int(stmt, 2);
    int monsterNext = sqlite3_column_int(stmt, 3);

    //value from DB
    p_montser->number = monster_number;
    p_montser->attack = monsterAttack;
    p_montser->maxLife = monsterMaxLife;
    p_montser->currentLife = monsterCurrentLife;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//Creation des monstres.
st_monsters * create_monsters(int nb_monster, int level_number)
{
    st_monsters *p_prev = NULL; //on créer un monstre à NULL (ce sera pour stocker / sauvegarder un autre monstre pdt le traitement)
    //printf("DEBUG :: create_monsters func begin\n");
    //on créer et alloue le premier monstre.
    st_monsters *p_first_monster = malloc(sizeof(st_monsters));

    //on init ses stats (et on passe le save_id pour récup les stats de la save si on as choisis une save). Aussi le lvl_number, car dans cas ou pas save, le lvl permet d'augmenter leurs stats au fur et à mesure.
    init_stats_monster(p_first_monster, level_number);

    //printf("DEBUG :: alloc p_monster [%d] = %p \n", p_first_monster->number, p_first_monster); //debug.

    //List chain manage
    p_first_monster->p_next = NULL; // monstre suivant init à NULL.
    p_prev = p_first_monster; //p_prev sauvegarde le premier.

    //création des monstres suivants.
    for (int i = 1; i < nb_monster; i++)
    {
        st_monsters * p_next = malloc(sizeof(st_monsters)); // p_next = nv monstre. On l'alloue.
        init_stats_monster (p_next,level_number); // on init leurs stats.
        //printf("DEBUG :: alloc p_monster [%d] = %p \n", p_next->number, p_next); //débug.

        //List chain manage
        p_next->p_next = NULL; // next = null.
        p_prev->p_next = (int *) p_next; //p_prev (qui contient le p_first), son next = p_next (celui que l'on créer actuellement).
        p_prev = p_next; //p_prev devient p_next. Donc le monstre actuellement créer. Pour que prochain tour de boucle, ca s'incrémente.
    }

    //*p_nb_monster = nb_monster; //nb de monstres.
    //printf("FIN FUNC\n");
    return p_first_monster; //return du p_first, puisque tout pars de lui.
};

st_monsters * create_monsters_save(st_monsters *p_monster, int save_id, int num_monster)
{
    //st_monsters *p_prev = p_monster; // p_prev = le monstre passer en param

    // si le premier est == NULL c'est aucun n'est créer. (pas de liste chainer etc..)
    if (p_monster == NULL)
    {
        p_monster = malloc(sizeof(st_monsters)); // donc on l'alloue

        //on init ses stats
        //printf("DEBUG :: alloc p_monster  IF NULL [%d] = %p \n", num_monster, p_monster); //debug.
        init_stats_monster_save(p_monster, save_id, num_monster);

        // le monstre suivant est NULL (on fait ca car si jamais on n'a qu'un seul monstre dans le niveau, on termine la liste ici).
        p_monster->p_next = NULL; // monstre suivant init à NULL.
        //return p_monster; // et on retourne le monstre.
    }
    else // mais si le monstre passer en param n'est pas null. C'est qu'une liste existe déjà. Donc ce n'est pas le premier de la liste.
    {
        st_monsters *p_prev = p_monster;

        //p_prev = p_monster; //p_prev sauvegarde le monstre.

        while(p_prev->p_next != NULL) // tant que le monstre suivant n'est pas null (donc tant que l'on arrive pas au dernier de la liste).
        {
            p_prev = (st_monsters *) p_prev->p_next; // le monstre devient le suivant (donc le denier en gros)
        }

        st_monsters* p_next = malloc(sizeof(st_monsters)); // nouveau monstre allouer.

        //printf("DEBUG :: alloc p_monster [%d] = %p \n", num_monster, p_next); //débug.
        init_stats_monster_save(p_next, save_id, num_monster); // on init ses stats.

        //List chain manage
        p_next->p_next = NULL; // next = null.
        p_prev->p_next = (int *) p_next;

        //return p_monster;
    }
    return p_monster;
};

// delete TOUS les monstres
int delete_all_monster(st_monsters *p_monster)
{
    do
    {
        int *p_next = (int *) p_monster->p_next; // pointeur d'int p_next = le next monsters.
        //printf("DEBUG :: free p_monster %d = %p \n", p_monster->number, p_monster); //debug
        free(p_monster); //on le free
        p_monster = (st_monsters *) p_next; //p_monster++
    }
    while (p_monster != NULL);
    return 0;
}

//delete UN monstre (lorsque l'on en tue un)
st_monsters *delete_the_monster(st_monsters *p_first_monster, st_monsters *p_the_monster)
{
    if (p_the_monster == p_first_monster) //Si le monstre (en paramètre, donc celui qu'on tue) == au premier de la liste.
    {
        st_monsters *p_new_first = (st_monsters *) p_the_monster->p_next; // alors notre nouveau premier monstre devient le deuxième.
        free(p_the_monster); //puisque qu'on delete le premier.
        return p_new_first; // On retourne notre new first.
    }
    else
    {
        st_monsters *p_monster = (st_monsters *) p_first_monster->p_next; //même opération. Comme on sait que ce n'est pas le premier, qui est contenu dans p_monster.
        st_monsters *p_prev = p_first_monster; // On stock tout de même le premier.

        while (p_the_monster != NULL) //On parcours toute notre liste.
        {
            // même opération que pour le premier. si le p_monster = au monstre en para (donc celui qu'on as tuer, et qui n'est pas notre first).
            if (p_monster == p_the_monster)
            {
                p_prev->p_next = p_monster->p_next; // p_prev (qui contient le premier) devient le next de celui que l'on désalloue.
                free(p_monster); // on libère le monstre.
                return p_first_monster;
            }
            p_prev = p_monster; //p_prev (qui est le premier monstre), devient le suivant.
            p_monster = (st_monsters *) p_monster->p_next; // et on donne l'adresse du suivant, sinon on ne peut plus parcourir, et accéder aux suivants à chaque fois.
        }

    }
    printf("Aucun monstre trouver. \n"); // si pas trouvé, bah pas trouver.
    return NULL;
}

// chercher un monstre. Pour choisir celui que l'on veut attaquer.
st_monsters *searchMonster(st_monsters *p_first_monster)
{
    int nb; //number du monstre choisis.
    int maxNb = 0;
    st_monsters *p_monster = p_first_monster; // p_monster contient notre first monster.
    // parcours de la liste chainé.
    while (p_monster != NULL)
    {
        printf("Monstre %d (%d/%d) \n", p_monster->number, p_monster->currentLife,
               p_monster->maxLife); // affiche chaque monstre.
        if (p_monster->number > maxNb) { // si le ->number du monstre est sup à MaxNB ;
            maxNb = p_monster->number; // maxNb = le ->number du monstre. (Ici maxNb est un compteur, qui nous permet de savoir le nombre de monstre, comme à chaque fois le nombre peut varié.
        }
        p_monster = (st_monsters *) p_monster->p_next; // i++.
    }

    printf("Quelle monstre attaquer ? :"); // on demande lequel on veut attaquer.
    scanf("%d", &nb);

    p_monster = p_first_monster; // on remet p_monster au premier monstre.

    // parcours de la liste etc..
    while (p_monster != NULL)
    {
        if (p_monster->number == nb) { //
            return p_monster; // si le monstre->number correspond au num choisis, on le retourne (comme une crêpe).
        }
        p_monster = (st_monsters *) p_monster->p_next; // i++
    }
    return NULL; // sinon on return null, la faudrait générer une erreur, ou un message mais azi.
}

//affichage des monstres
int display_monsters(st_monsters *p_first_monster)
{
    int nb_monster = 0;
    st_monsters *p_monster = p_first_monster;

    while (p_monster != NULL)
    {
        if (p_monster->currentLife > 0)
        {  // Vérifier si le monstre est en vie
            nb_monster++;  // Augmenter le compteur de monstres vivants
            printf("Monstre %d\n", p_monster->number);
            printf("Vie : %d / %d\n", p_monster->currentLife, p_monster->maxLife);
            printf("Attaque : %d\n", p_monster->attack);
            printf("Défense : %d\n", p_monster->defense);
            printf("-----\n");
        }
        p_monster = (st_monsters *) p_monster->p_next;
    }
    printf("Number of Monsters: %d\n", nb_monster);
    return 0;
}