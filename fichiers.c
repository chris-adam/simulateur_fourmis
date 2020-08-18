#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constantes.h"
#include "jeu.h"
#include "fichiers.h"
#include "parametres.h"
#include "graphisme.h"
#include "carte.h"


int chargerCarte(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int numeroCarte)
{
    FILE* fichier = NULL;
    char ligneFichier[LARGEUR_CARTE * HAUTEUR_CARTE + 1] = {0};
    int i = 0, j = 0;

    switch(numeroCarte)
    {
        case 1:
            fichier = fopen("carte/carte1.txt", "r");
            break;
        case 2:
            fichier = fopen("carte/carte2.txt", "r");
            break;
    }

    if (fichier == NULL)
        return 0;

    fgets(ligneFichier, LARGEUR_CARTE * HAUTEUR_CARTE + 1, fichier);

    for (i = 0 ; i < HAUTEUR_CARTE ; i++)
    {
        for (j = 0 ; j < LARGEUR_CARTE ; j++)
        {
            switch (ligneFichier[(i * LARGEUR_CARTE) + j])
            {
                case '0':
                    carte[PHYSIQUE][j][i] = 0;
                    break;
                case '1':
                    carte[PHYSIQUE][j][i] = 1;
                    break;
                case '2':
                    carte[PHYSIQUE][j][i] = 2;
                    break;
                case '3':
                    carte[PHYSIQUE][j][i] = 3;
                    break;
                case '4':
                    carte[PHYSIQUE][j][i] = 4;
                    break;
                case '5':
                    carte[PHYSIQUE][j][i] = 5;
                    break;
                default:
                    carte[PHYSIQUE][j][i] = 666;
                    break;
            }
        }
    }

    return 1;
}
