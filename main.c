#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_MONSTER 5 // valeur "interne" au proramme


//-----------------PARTIE PLAYER-----------------
typedef struct st_player
{
    int maxLife; //stats player
    int currentLife;
    int nb_attack_per_round;
    int attack;
    int defense;
    int weapons; // weapons à revoir, j'ai mis ca comme ça histoire de, mais surement une structure rien que pour les armes, comme il y'a plusieurs armes, avec des statistiques différentes etc...
} st_player;


st_player* create_player (void) // fonction de type * st_player pour créer le joueur au lancement du prg. Le void, on attend rien en paramètre d'entrée.
{
    st_player* p_player = malloc(sizeof(st_player)); //p_player = le joueur en gros.
    printf("debug :: alloc p_player [%d] = %p \n",1,p_player); // printf pour débug, vérifier que le joueur est bien créer/allouer.

    srand(time(NULL)); //pour que chaque génération diffère des précédentes / suivantes.
    p_player->maxLife = 50;//rand() % 50; // la vie du MyPlayer. Valeur de base, à changer plus tard.
    p_player->currentLife = p_player->maxLife;
    p_player->nb_attack_per_round = rand() % (10 + 1); // Attaque par round, c'était évoqué dans le sujet, donc je l'ai mis, mais faudras approfondir sur ce point, et se mettre d'accord sur comment ca marche cette historie de d'attaque par round. (pour moi un round = une attaque chacun (MyPlayer, monstre(s) ??).
    p_player->attack = rand() % (10 + 1);
    p_player->defense = rand() % (10 + 1);
    p_player->weapons = rand() % 10; // comme évoqué, j'ai foutu ca la, mais faudras faire différemnt pour les armes.

    return p_player;
}


int delete_player (st_player * p_player) //Supprime le joueur.
{
    printf("debug :: free p_player %d = %p \n",1,p_player); // débug, pour vérifier qu'il est bien désallouer, faudras appeler cette fonction, quand notre MyPlayer, mourra par exemple, ce sera la fin de la game.

    free (p_player);
    return 0;
}
//-----------------FIN PARTIE PLAYER-----------------


//-----------------PARTIE MONSTERS-----------------
typedef struct st_monsters
{
    int number; // numero du monstre (comme le nombre de monstre est aléatoire, on lui donne un nombre, pour la suite.

    int maxLife; //stats monstres
    int currentLife;
    int attack;
    int defense;

    int * p_next; // Pointeur d'int pour passer de monstre en monstre, voir suite.
} st_monsters;


st_monsters * create_monsters (int * p_nb_monster) // même chose pour les monstres, sauf qu'on prend le nb de monstres en entrée.
{
    int nb_monster = 0; //init pour plus tard.
    st_monsters* p_prev = NULL; //les monstres qui suivront le premier, z'allez comprendre tkt.

    srand(time(NULL)); //déjà vu.

    nb_monster = 2; //rand() % MAX_MONSTER; //on génère entre 0 et 5 monstres (valeurs changer, mais jme dis que si y'a 15 monstres, pour les afficher en invite de commandes ca fait bcp la non)
    if (nb_monster == 0) nb_monster = 1; //évidemment si le nb_monster = 0, on passe à 1. Au minimum un monstre.

    st_monsters* p_first_monster = malloc(sizeof(st_monsters)); // allocution du premier monstre.
    p_first_monster->number = 1; // C'est le premier monstre, donc logique c'est 1 son nombre. Z'allez comprendre tkt.
    printf("debug :: alloc p_monster [%d] = %p \n",p_first_monster->number,p_first_monster); //pareil, pour vérifier que tout marche bien.
    p_first_monster->maxLife = rand() % (20 + 1);
    p_first_monster->currentLife = p_first_monster->maxLife;
    p_first_monster->attack = rand() % (10 + 1); //toutes les valeurs, évidemment faudras qu'on se mette d'accord la dessus, la j'ai fais comme ca, pour avoir une base.
    p_first_monster->defense = rand() % 10;

    p_first_monster->p_next = NULL; // le p_next (donc monstre suivant) est à NULL pour l'instant, on l'init juste, pas le choix.

    if (nb_monster > 1) // si le nombre de monstre est = 1, le désalouer no problem, mais si nb_monster > 1, pour désalouer les monstres, j'ai fais comme ça : (bien entendu si vous avez plus simple, je suis preneur ..)
    {
        p_prev = p_first_monster; //p_prev devient p_first_monster, ici on sauvegarde le premier monstre dans cette variable. (d'ou le "prev", prev = précédent).
        for (int i = 1; i < nb_monster; i++) // on parcours les monstres (si il y'en as plus d'un), sinon ca redéfinit simplement les valeurs du premier, ce qui ne change rien au final.
        {
            st_monsters *p_next = malloc(sizeof(st_monsters)); // on leurs alloue tous un espace mémoire.
            p_next->number = i+1; // le nombre DU monstre, s'incrémente, donc 2 ici pour le monstre 2, 3 pour le troisème etc...
            printf("debug :: alloc p_monster [%d] = %p \n",p_next->number, p_next); // même chose, débug.
            p_next->maxLife = rand() % (20 + 1);
            p_next->currentLife = p_next->maxLife;
            p_next->attack = rand() % (10 + 1); // pareil que tout à l'heure ici, on leurs donne des stats aléatroire.
            p_next->defense = rand() % 10;
            p_next->p_next = NULL; // on init à NULL.
            p_prev->p_next = (int *)p_next; //le (int *) c'est pour donner à p_next le type int, de base il était de type st_monsters. Et on donne p_prev->p_next la valeur int* p_next, qui correspond à l'adresse du monstre suivante. (rappelez vous les listes, chaque monstre est une valeur de la liste, et pour la parcourir, il faut accéder à la l'adresse de l'emplacement suivant).
            p_prev = p_next; //enfin notre p_prev devient notre p_next, donc le premier monstre devient le suivant, et ainsi de suite.
        }
    }
    //retourne le nombre et le pointeur
    *p_nb_monster = nb_monster;
    return p_first_monster;
};


int delete_monsters (st_monsters * p_monster) // Supprime TOUT les monstres..
{
    do
    {
        int * p_next = (int  *)p_monster->p_next; // on reforce le type, pck à l'entréer on doit lui passer toute notre stuct
        printf("debug :: free p_monster %d = %p \n",p_monster->number,p_monster);
        free (p_monster);
        p_monster = (st_monsters *)p_next; //on lui redonne la valeur de p_next, pour le suivant.
    }
    while (p_monster != NULL);
    return 0;
}


st_monsters * delete_the_monster (st_monsters * p_first_monster, st_monsters* p_the_monster) // Supprime UN monstre.
{
    if(p_the_monster == p_first_monster){ //si le monstre est = au premier (dans la liste chainé)
        //memset(p_first_monster, 0, sizeof(st_monsters));
        st_monsters * p_new_first = (st_monsters *)p_the_monster->p_next; // p_new_first = le deuxième monstre.
        free(p_the_monster); // on libère le monstre (comme il est le premier)
        return p_new_first; //et on retourne le p_new_first, le nouveau premier monstre de notre liste chainé
    }
    else{
        st_monsters * p_monster = (st_monsters *)p_first_monster->p_next; //p_monster = le monstre suivant (ici on ne sait pas ou on est dans la liste, juste pas premier)
        st_monsters * p_prev = p_first_monster; // p_prev contient notre premier monstre.
        while(p_the_monster != NULL){ // tant qu'on n'a pas parcouru toute la liste chainé.
            if(p_monster == p_the_monster){ // si le monstre (qui est rentrer en paramètre), est égale a un monstre dans la liste parcouru ;
                p_prev->p_next = p_monster->p_next; // p_prev (qui contient le premier monstre), son suivant devient celui d'apères supprimer. (en gros si on supp le 2, on relie le 1 au 3).
                free(p_monster); // on libère le monstre.
                return p_first_monster; //
            }
            p_prev = p_monster; //p_prev (qui est le premier monstre), devient le suivant. (c'est un i++ en gros la).
            p_monster = (st_monsters *)p_monster->p_next; // et on donne l'adresse du suivant, sinon on ne peut plus parcourir, et accéder aux suivants à chaque fois.

        }
    }
    printf("Not found \n"); // si pas trouvé, bah pas trouver.
    return NULL;
}
//-----------------FIN PARTIE MONSTRES-----------------


//-----------------PARTIE AFFICHAGE DES STATS-----------------
int display_player (st_player* p_player) // affichage des statistiques du joueur.
{
    printf("Vie : %d / %d point de vie. \n",p_player->currentLife, p_player->maxLife);
    printf("Attaque :  : %d \n", p_player->attack );
    printf("Armes : %d \n", p_player->weapons);
    return 0;
}

int display_monsters (st_monsters* p_first_monster) //affichage des stats des monstres.
{
    int nb_monster = 0;
    st_monsters* p_monster = p_first_monster;

    while (p_monster != NULL) {
        if (p_monster->currentLife > 0) {  // Vérifier si le monstre est en vie
            nb_monster++;  // Augmenter le compteur de monstres vivants
            printf("Monstre %d\n", p_monster->number);
            printf("Vie : %d / %d\n", p_monster->currentLife, p_monster->maxLife);
            printf("Attaque : %d\n", p_monster->attack);
            printf("Défense : %d\n", p_monster->defense);
            printf("-----\n");
        }
        p_monster = (st_monsters*)p_monster->p_next;
    }
    printf("Number of Monsters: %d\n", nb_monster);
    return 0;
}
//-----------------FIN PARTIE AFFICHAGE DES STATS-----------------


//-----------------PARTIE FIGHT-----------------
st_monsters * searchMonster(st_monsters* p_first_monster) // func pour chercher un monstre (pour choisir celui qu'on va attaquer). Elle a été faite pour ca, mais il y'a possibilité que l'on soit amené à l'utilsier pour autre chose, donc son contenu pourra changer.
{
    int nb; //number du monstre choisis.
    int maxNb = 0; //
    st_monsters* p_monster = p_first_monster; // p_monster contient notre first monster.
    while(p_monster != NULL) { // parcours de la liste chainé.
        printf("Monstre %d (%d/%d) \n", p_monster->number, p_monster->currentLife, p_monster->maxLife); // affiche chaque monstre.
        if(p_monster->number > maxNb){ // si le ->number du monstre est sup à MaxNB ;
            maxNb = p_monster->number; // maxNb = le ->number du monstre. (Ici maxNb est un compteur, qui nous permet de savoir le nombre de monstre, comme à chaque fois le nombre peut varié.
        }
        p_monster = (st_monsters  *)p_monster->p_next; // i++.
    }

    printf("Lequel ? :"); // on demande lequel on veut attaquer.
    scanf("%d", &nb);

    p_monster = p_first_monster; // on remet p_monster au premier monstre.

    while(p_monster != NULL){ // parcours de la liste etc..
        if(p_monster->number == nb) { //
            return p_monster; // si le monstre->number correspond au num choisis, on le retourne (comme une crêpe).
        }
        p_monster = (st_monsters *)p_monster->p_next; // i++
    }
    return NULL; // sinon on return null, la faudrait générer une erreur, ou un message mais azi.
}


st_monsters * fight_round(st_player* p_player, st_monsters* p_first_monster){ // le FIGHT.

    st_monsters * p_monster_found; // le monstre que l'on veut attaquer.
    p_monster_found = searchMonster(p_first_monster); // on utilise la func pour choisir le monstre, et on stock ce monstre.
    if(p_monster_found == NULL) // si marche pas
    {
        printf("No one found. \n");
        return p_first_monster; // on renvoit le le first.
    }

    p_monster_found->currentLife -= p_player->attack; // opération, on diminue la vie du monstre, en fonction de l'attaque du joueur.

    if(p_monster_found->currentLife < 0) p_monster_found->currentLife = 0; // à partir du moment ou la vie du monstre < 0, on la remet à zéro. (au lieu d'avoir [random] -8, on met 0, plus clean.

    if(p_monster_found->currentLife == 0){ // si la vie du monstre == 0
        printf("Monstre éliminé %d \n", p_monster_found->number);
        return delete_the_monster(p_first_monster, p_monster_found); //on le sup.
    }
    else{
        printf("Le monstre %d vient de prendre %d , sa vie est désomrais de %d \n",p_monster_found->number, p_player->attack, p_monster_found->currentLife); //sinon il prend simplement les dégats.
        return p_first_monster; // et on retourne le premier monstre.
    }
}
//-----------------FIN PARTIE FIGHT-----------------


//-----------------PARTIE INVENTAIRE-----------------

void display_inventory(st_player* p_player){
    int option = 1;

    while (option != 4){
        printf("1. Armes\n");
        printf("2. Armures\n");
        printf("3. Potions\n");
        printf("4. Retour\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Vous accedez aux armes\n");
                break;
            case 2:
                printf("Vous accedez aux armures\n");
                break;
            case 3:
                printf("Vous accedez aux potions\n");
                break;
            case 4:
                break;
        }
    }
}
//-----------------FIN PARTIE INVENTAIRE-----------------



int main(void)
{
    int nb_monster = 0;
    int choixMenu = 1;

    st_player* p_player = create_player(); // on créer le joueur.
    st_monsters * p_first_monster = create_monsters(&nb_monster); //on créer nos monstres.

    while (choixMenu != 4)
    {
        printf("---Menu-- - \n");
        printf("1.stat joueur \n");
        printf("2.stat monsters \n");
        printf("3.game \n");
        printf("4.sortir \n");
        printf("5.Inventaire\n");
        scanf("%d", &choixMenu);

        switch(choixMenu) {
            case 1: {
                display_player(p_player);
                break;
            }
            case 2: {
                display_monsters(p_first_monster);
                break;
            }
            case 3:
                /*Game*/
                p_first_monster = fight_round(p_player, p_first_monster);
                break;
            case 4:
                break;
            case 5:
                display_inventory(p_player);
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
