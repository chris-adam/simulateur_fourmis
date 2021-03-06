#ifndef GRAPHISME_H_INCLUDED
#define GRAPHISME_H_INCLUDED

int eau(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j);
int coton(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j);
int verre(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j);
int bord(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j);

enum{EAU_MILIEU, EAU_HAUT, EAU_DROITE, EAU_BAS, EAU_GAUCHE, EAU_HAUT_DROITE, EAU_HAUT_GAUCHE, EAU_BAS_DROITE, EAU_BAS_GAUCHE, TAILLE_TABLEAU_EAU};
enum{COTON_MILIEU, COTON_HAUT, COTON_BAS, COTON_GAUCHE, COTON_DROITE, TAILLE_TABLEAU_COTON};
enum{VERRE_HAUT, VERRE_BAS, VERRE_GAUCHE, VERRE_DROITE, TAILLE_TABLEAU_VERRE};
enum{BORD_HAUT, BORD_DROITE, BORD_BAS, BORD_GAUCHE, BORD_HAUT_DROITE, BORD_HAUT_GAUCHE, BORD_BAS_DROITE, BORD_BAS_GAUCHE, TAILLE_TABLEAU_BORD};


#endif // GRAPHISME_H_INCLUDED
