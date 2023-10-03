#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MONSTER 5 // valeur "interne" au proramme


typedef struct st_monsters
{
    /*numero du monstre (comme le nombre de monstre est aléatoire, on lui donne un nombre, pour la suite.*/
    int number;

    /*Statistique du monstre.*/
    int life;
    int maxAttack;
    int minAttack;
    int defense;

    /*Pointeur d'int pour passer de monstre en monstre, voir suite. */
    int * p_next;
} st_monsters;

typedef struct st_player
{
    //MyPlayer
    int life;
    int nb_attack_per_round;
    int weapons; // weapons à revoir, j'ai mis ca comme ça histoire de, mais surement une structure rien que pour les armes, comme il y'a plusieurs armes, avec des statistiques différentes etc...
} st_player;

st_player * create_player (void) // fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
{
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.
    printf("debug :: alloc p_player [%d] = %p \n",1,p_player); // printf pour débug, vérifier que le joueur est bien créer/allouer.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.
    p_player->life = rand() % 1000; // la vie du MyPlayer. Valeur de base, à changer plus tard.
    p_player->nb_attack_per_round = rand() % 1000; // Attaque par round, c'était évoqué dans le sujet, donc je l'ai mis, mais faudras approfondir sur ce point, et se mettre d'accord sur comment ca marche cette historie de d'attaque par round. (pour moi un round = une attaque chacun (MyPlayer, monstre(s) ??).
    p_player->weapons = rand() % 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.

    return p_player;
}

st_monsters * create_monsters (int * p_nb_monster) // même chose pour les monstres, sauf qu'on prend le nb de monstres en entrée.
{
    int nb_monster = 0; //init pour plus tard.
    st_monsters* p_prev = NULL; //les monstres qui suivront le premier, z'allez comprendre tkt.

    srand(time(NULL)); //déjà vu.

    nb_monster = rand() % MAX_MONSTER; //on génère entre 0 et 5 monstres (valeurs changer, mais jme dis que si y'a 15 monstres, pour les afficher en invite de commandes ca fait bcp la non)
    if (nb_monster == 0) nb_monster = 1; //évidemment si le nb_monster = 0, on passe à 1. Au minimum un monstre.

    st_monsters* p_first_monster = malloc(sizeof(st_monsters)); // allocution du premier monstre.
    p_first_monster->number      = 1; // C'est le premier monstre, donc logique c'est 1 son nombre. Z'allez comprendre tkt.
    printf("debug :: alloc p_monster [%d] = %p \n",p_first_monster->number,p_first_monster); //pareil, pour vérifier que tout marche bien.
    p_first_monster->life        = rand() % 1000;
    p_first_monster->maxAttack   = rand() % 1000;
    p_first_monster->minAttack   = rand() % 10; //toutes les valeurs, évidemment faudras qu'on se mette d'accord la dessus, la j'ai fais comme ca, pour avoir une base.
    p_first_monster->defense     = rand() % 100;

    p_first_monster->p_next      = NULL; // le p_next (donc monstre suivant) est à NULL pour l'instant, on l'init juste, pas le choix.

    if (nb_monster > 1) // si le nombre de monstre est = 1, le désalouer no problem, mais si nb_monster > 1, pour désalouer les monstres, j'ai fais comme ça : (bien entendu si vous avez plus simple, je suis preneur ..)
    {
        p_prev = p_first_monster; //p_prev devient p_first_monster, ici on sauvegarde le premier monstre dans cette variable. (d'ou le "prev", prev = précédent).
        for (int i = 1; i < nb_monster; i++) // on parcours les monstres (si il y'en as plus d'un), sinon ca redéfinit simplement les valeurs du premier, ce qui ne change rien au final.
        {
            st_monsters *p_next = malloc(sizeof(st_monsters)); // on leurs alloue tous un espace mémoire.
            p_next->number = i+1; // le nombre DU monstre, s'incrémente, donc 2 ici pour le monstre 2, 3 pour le troisème etc...
            printf("debug :: alloc p_monster [%d] = %p \n",p_next->number,p_next); // même chose, débug.
            p_next->life = rand() % 1000;
            p_next->maxAttack = rand() % 1000;
            p_next->minAttack = rand() % 10; // pareil que tout à l'heure ici, on leurs donne des stats aléatroire.
            p_next->defense = rand() % 100;

            p_next->p_next = NULL; // on init à NULL.
            p_prev->p_next = (int *)p_next; //le (int *) c'est pour donner à p_next le type int, de base il était de type st_monsters. Et on donne p_prev->p_next la valeur int* p_next, qui correspond à l'adresse du monstre suivante. (rappelez vous les listes, chaque monstre est une valeur de la liste, et pour la parcourir, il faut accéder à la l'adresse de l'emplacement suivant).
            p_prev = p_next; //enfin notre p_prev devient notre p_next, donc le premier monstre devient le suivant, et ainsi de suite.
        }
    }

    //retourne le nombre et le pointeur
    *p_nb_monster = nb_monster;
    return p_first_monster;
};

int delete_player (st_player * p_player) //fonction pour désallouer notre MyPlayer.
{
    printf("debug :: free p_player [%d] = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.

    free (p_player);
    return 0;
}

int delete_monsters (st_monsters * p_monster) // même chose pour les monstres.
{
    do
    {
        int * p_next = (int  *)p_monster->p_next; // on reforce le type, pck à l'entréer on doit lui passer toute notre stuct
        printf("debug :: free p_monster [%d] = %p \n",p_monster->number,p_monster);
        free (p_monster);
        p_monster = (st_monsters *)p_next; //on lui redonne la valeur de p_next, pour le suivant.
    }
    while (p_monster != NULL);

    return 0;
}

//--------- AFFICHAGE (début de menu, juste histoire d'avoir un point de départ).

int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    //affichage joueur
    printf("Le joueur a point de vie                   : %d \n", p_player->life);
    printf("Le joueur a un nombre d'attaque par tour   : %d \n", p_player->nb_attack_per_round );
    printf("Le monstre un nombre d'arme                : %d \n", p_player->weapons);
    return 0;
}

int display_monsters (st_monsters* p_first_monster, int nb_monster) //affichage des stats des monstres.
{
    printf("number of monster : %d \n",nb_monster);
    if (nb_monster > 0)
    {
        st_monsters * p_monster = p_first_monster;
        do
        {
            printf("----- \n");
            printf("Monstre %d \n", p_monster->number);
            printf("Le monstre a point de vie                   : %d \n", p_monster->life);
            printf("Le monstre a une puissance d'attaque max de : %d \n", p_monster->maxAttack);
            printf("Le monstre a une puissance d'attaque min de : %d \n", p_monster->minAttack);
            printf("Le monstre a une defense de                 : %d \n", p_monster->defense);
            p_monster = (st_monsters *)p_monster->p_next;
        }
        while (p_monster);
        printf("----- \n");
    }
    return 0;
}

int game (st_player* p_player, st_monsters* p_fist_monster) //simple fonction pour faire un début de combat, j'ai simplement fait que si le joueur a plus de vie que le monstre, il gagne, évidemment faut bien alimenter le truc, avec les point de vie tout ca, mais c un point de départ.
{
    int retour = 1;//gagné

    st_monsters * p_monster = p_fist_monster;
    do
    {
        if ( p_player->life < p_monster->life)
            retour = 0;//perdu
        p_monster = (st_monsters *)p_monster->p_next;
    }
    while (p_monster);


    return retour;
}

int main()
{
    int nb_monster = 0;
    int choixMenu = 1;

    st_player* p_player = create_player(); // on créer le joueur.
    st_monsters * p_first_monster = create_monsters(&nb_monster); //on créer nos monstres.

    while (choixMenu != 4)
    {
        printf("-----------Menu-----------\n");
        printf("1. Statistiques du joueur\n");
        printf("2. Statistiques des monstres\n");
        printf("3. Jouer\n");
        printf("4. Quitter\n");
        scanf("%d", &choixMenu);

        switch(choixMenu) {
            case 1: {
                display_player(p_player);
                break;
            }
            case 2: {
                display_monsters(p_first_monster, nb_monster);
                break;
            }
            case 3:
                /*Game*/
                if (0 == game (p_player,p_first_monster))
                {
                    printf("PERDU \n");
                }
                else
                {
                    printf("GAGNE \n");
                }
                break;
            case 4:
                break;
        }
    }

    //Libere les montres*/
    delete_monsters(p_first_monster);
    //Libère le joueur.
    delete_player(p_player);

    //system("PAUSE");

    return 0;
}
