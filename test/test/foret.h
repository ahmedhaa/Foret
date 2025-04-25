

#ifndef FORET_H
#define FORET_H

#define VIDE    0
#define ARBRE   1
#define FEU     2
#define CENDRE  3
#define SIZE 100 /* Taille de la forêt */

/* variable globale */
extern int foret[SIZE][SIZE];

/* Initialisation de la forêt */
void init_foret(int f[][SIZE]);

/* logique  */
void calcul_etape(int f[][SIZE]);

/* propagation du feu */
int voisinage_4(int f[][SIZE], int x, int y);
int voisinage_8(int f[][SIZE], int x, int y);

#endif
