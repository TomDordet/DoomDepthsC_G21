#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Player.h"
#include "Display.h"
#include "level.h"
#include "Monster.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Fonction pour effacer l'écran sur macOS
void clearScreen() {
#ifdef _WIN32 // Check if the platform is Windows
    system("cls"); // Use "cls" to clear the screen in Windows
#else
    system("clear"); // Use "clear" to clear the screen on macOS and Unix-like systems
#endif
}



//void affichageTerminale() {
    //int terminal_width;

//#ifdef _WIN32
  //  CONSOLE_SCREEN_BUFFER_INFO csbi;
    //GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    //terminal_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
// #else
   // struct winsize w;
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Obtenez la taille du terminal
   // terminal_width = w.ws_col;
// #endif
int choixMonstre;

char *hero[] = {
        "  / \\                    ",
        "  | |                    ",
        "  |.|                    ",
        "  |.|                    ",
        "  |:|      __            ",
        ",_|:|_,   /  )           ",
        "  (Oo    / _I_           ",
        "   +\\ \\  || __|          ",
        "      \\ \\||___|          ",
        "        \\ /.:.\\-\\        ",
        "         |.:. /-----\\    ",
        "         |___|::oOo::|   ",
        "         /   |:<_T_>:|   ",
        "        |_____\\ ::: /    ",
        "         | |  \\ \\:/      ",
        "         | |   | |       ",
        "         \\ /   | \\___    ",
        "         / |   \\_____\\   ",
        "         `-'"
};


char *monstre[] = {
        "      .-.       ",
        "     (o.o)      ",
        "      |=|       ",
        "     __|__      ",
        "   //.=|=.\\\\    ",
        "  // .=|=. \\\\   ",
        "  \\\\ .=|=. //   ",
        "   \\\\(_=_)//    ",
        "    (:| |:)     ",
        "     || ||      ",
        "     () ()      ",
        "     || ||      ",
        "     || ||      ",
        "    ==' '==     ",
        "",
        "",
        "",
        "",
        ""


};



void afficherHero() {
    printf("\n\n");
    for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++) {

        printf("%s\n", hero[i]);
    }
    printf("\n\n");
}



void afficherPersonnages(st_monsters* p_monster) {
    // Affichage personnage
    int nb_monstre = 0;
   st_monsters* monstres = p_monster;
    while (monstres != NULL){
        nb_monstre ++;
        monstres = (st_monsters*) monstres->p_next;
    }


    for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++) {
        if (nb_monstre == 1) {
            printf("%s\t\t\t\t\t%s\n", hero[i], monstre[i]);
        } else if (nb_monstre == 2) {
            printf("%s\t\t\t\t\t%s\t%s\n", hero[i], monstre[i], monstre[i]);
        } else if (nb_monstre == 3) {
            printf("%s\t\t\t\t\t%s\t%s\t%s\n", hero[i], monstre[i], monstre[i], monstre[i]);
        } else {
            printf("%s\t\t\t\t\t%s\t%s\t%s\t%s\n", hero[i], monstre[i], monstre[i], monstre[i], monstre[i]);

        }
    }
}

void display_health_bar(st_player *p_player) {
    int currentLife = p_player->currentLife;
    int maxLife = p_player->maxLife;
    // Longueur de la barre de vie
    int barLength = 100;

    int healthBarSize = (currentLife * barLength) / maxLife;
    printf("Vie: [");
    for (int i = 0; i < barLength; i++) {
        if (i < healthBarSize) {
            printf("\x1b[31m#\x1b[0m");
        } else {
            printf("\x1b[31m-\x1b[0m");
        }
    }
    printf("] %d/%d\n", currentLife, maxLife);
}


void affichageMenu(st_player *p_player, st_monsters* p_monster){
    display_health_bar(p_player);
    printf("\n \n");
    afficherPersonnages(p_monster);
    printf("\n \n");

}


void youWin(){
    printf(" __     __          __          ___       \n"
           " \\ \\   / /          \\ \\        / (_)      \n"
           "  \\ \\_/ ___  _   _   \\ \\  /\\  / / _ _ __  \n"
           "   \\   / _ \\| | | |   \\ \\/  \\/ / | | '_ \\ \n"
           "    | | (_) | |_| |    \\  /\\  /  | | | | |\n"
           "    |_|\\___/ \\__,_|     \\/  \\/   |_|_| |_|\n"
           "                                          \n"
           "                                          \n");
}

void doomDepths(){
    printf(" _____                            _            _   _         \n"
           " |  __ \\                          | |          | | | |        \n"
           " | |  | | ___   ___  _ __ ___   __| | ___ _ __ | |_| |__  ___ \n"
           " | |  | |/ _ \\ / _ \\| '_ ` _ \\ / _` |/ _ \\ '_ \\| __| '_ \\/ __|\n"
           " | |__| | (_) | (_) | | | | | | (_| |  __/ |_) | |_| | | \\__ \\\n"
           " |_____/ \\___/ \\___/|_| |_| |_|\\__,_|\\___| .__/ \\__|_| |_|___/\n"
           "                                         | |                  \n"
           "                                         |_| \n\n\n\n\n\n");
}

void gameOver(){
    printf("   _____                         ____                 \n"
           "  / ____|                       / __ \\                \n"
           " | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ \n"
           " | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|\n"
           " | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   \n"
           "  \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|  \n ");
}
