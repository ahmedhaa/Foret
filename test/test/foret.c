#include <stdlib.h>
#include <time.h>
#include "foret.h"

int foret[SIZE][SIZE];

// Initialisation de la forêt 
void init_foret(int f[][SIZE]) {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            f[i][j] = (rand() % 100 < 80) ? ARBRE : VIDE;
        }
    }
    // feu au centre
    f[SIZE / 2][SIZE / 2] = FEU;
}
// regle de  4-voisins
int voisinage_4(int f[][SIZE], int x, int y) {
    int feu = 0;
    if (x > 0 && f[x - 1][y] == FEU) feu++;  // Nord
    if (x < SIZE - 1 && f[x + 1][y] == FEU) feu++;  // Sud
    if (y > 0 && f[x][y - 1] == FEU) feu++;  // Ouest
    if (y < SIZE - 1 && f[x][y + 1] == FEU) feu++;  // Est
    return feu;
}
// regle de  8-voisins
int voisinage_8(int f[][SIZE], int x, int y) {
    int feu = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x + i >= 0 && x + i < SIZE && y + j >= 0 && y + j < SIZE && (i != 0 || j != 0)) {
                if (f[x + i][y + j] == FEU) {
                    feu++;
                }
            }
        }
    }
    return feu;
}

void calcul_etape(int f[][SIZE]) {
    int copie[SIZE][SIZE];  // Matrice temporaire 

    // Copier la forêt pour calculer le prochain état
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            copie[i][j] = f[i][j];
        }
    }

    // Règles de propagation du feu
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (copie[i][j]) {
            case ARBRE:
                // Si un arbre a au moins un voisin 4-voisin en feu, il prend feu avec une probabilité de 0.8
                if (voisinage_4(copie, i, j) > 0 && (rand() / (double)RAND_MAX) < 0.8) {
                    f[i][j] = FEU;
                }
                // Si un arbre a un voisin 8-voisin en feu (pas un 4-voisin), il prend feu avec une probabilité de 0.5
                else if (voisinage_8(copie, i, j) > voisinage_4(copie, i, j) && (rand() / (double)RAND_MAX) < 0.5) {
                    f[i][j] = FEU;
                }
                break;

            case FEU:
                // Un arbre en feu devient des cendres après 
                f[i][j] = CENDRE;
                break;

            case CENDRE:
                // Les cendres restent des cendres
                break;

            case VIDE:
                // Les cases vides ne changent pas
                break;
            }
        }
    }
}
