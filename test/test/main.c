#include <stdlib.h>
#include <time.h>
#include "ez-draw.h"
#include "foret.h"

#define C 5  /* Taille d'une case à l'écran en pixels */

//fenêtre créée avec la bibliothèque Ez-Draw
Ez_window win_global;

/* affichage */
void trace_foret(int f[][SIZE], Ez_window win) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = j * C;
            int y = i * C;

            switch (f[i][j]) {
            case VIDE:
                ez_set_color(ez_white);
                break;
            case ARBRE:
                ez_set_color(ez_green);
                break;
            case FEU:
                ez_set_color(ez_red);
                break;
            case CENDRE:
                ez_set_color(ez_grey); 
                break;
            }

            ez_fill_rectangle(win, x, y, x + C, y + C); // remplir un rectangle 
        }
    }
}

/* Fonction de rappel de l'événement Timer */
void timer_cb(Ez_event* ev) {
    calcul_etape(foret);           //Met à jour la forêt
    ez_send_expose(win_global);   // Redessine la fenêtre
    ez_start_timer(ev->win, 100); // Relance le timer après 100ms
}

/* Fonction de gestion des événements de la fenêtre */
void win_cb(Ez_event* ev) {
    switch (ev->type) {
    case Expose:
        trace_foret(foret, ev->win); // Redessine la forêt
        break;
    case TimerNotify:
        timer_cb(ev); // Mets à jour la forêt à chaque timer
        break;
    case KeyPress:
        break;
    case ButtonPress:
        if (ev->mb == 1) {  // Vérifie si le bouton gauche de la souris a été pressé)
            int x = ev->mx / C; //indices x 
            int y = ev->my / C; //indices y 

            if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
                if (foret[y][x] == ARBRE) {
                    foret[y][x] = FEU;  //tableau 2D, la ligne vient avant la colonne
                    printf("Incendie allume a (%d, %d)\n", x, y);
                }
            }
            ez_send_expose(ev->win);
        }
        break;
    }
}



int main() {
    if (ez_init() < 0) exit(1);

    init_foret(foret);

    win_global = ez_window_create(SIZE * C, SIZE * C, "Feuforet", win_cb);

    ez_window_dbuf(win_global, 1); // double buffering 

    ez_start_timer(win_global, 300); // Lance le timer pour la première fois

    ez_main_loop();
    exit(0);
}
