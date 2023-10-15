#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Monster.h"

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
            printf("Defense : %d\n", p_monster->defense);
            printf("-----\n");
        }
        p_monster = (st_monsters*)p_monster->p_next;
    }
    printf("Number of Monsters: %d\n", nb_monster);
    return 0;
}