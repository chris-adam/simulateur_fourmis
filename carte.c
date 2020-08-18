#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constantes.h"
#include "jeu.h"
#include "fichiers.h"
#include "parametres.h"
#include "graphisme.h"
#include "carte.h"


int humidite(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE])
{
    int i = 0, j = 0, humidite = 0;


    for(i = 0 ; i < LARGEUR_CARTE ; i++)
    {
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
        {
            if(carte[PHYSIQUE][i][j] == EAU)
                carte[HUMIDITE][i][j] = 100;


            if(carte[PHYSIQUE][i][j] == COTON)
            {
                humidite = 0;

                humidite = carte[HUMIDITE][i+1][j];

                if(carte[HUMIDITE][i-1][j] > humidite)
                    humidite = carte[HUMIDITE][i-1][j];
                else if(carte[HUMIDITE][i][j+1] > humidite)
                    humidite = carte[HUMIDITE][i][j+1];
                else if(carte[HUMIDITE][i][j-1] > humidite)
                    humidite = carte[HUMIDITE][i][j-1];

                if(humidite == 100)
                    carte[HUMIDITE][i][j] = 100;
            }

            if(carte[PHYSIQUE][i][j] == VERRE || carte[PHYSIQUE][i][j] == TUBE)
            {
                humidite = 0;

                humidite = carte[HUMIDITE][i+1][j];

                if(carte[HUMIDITE][i-1][j] > humidite)
                    humidite = carte[HUMIDITE][i-1][j];
                else if(carte[HUMIDITE][i][j+1] > humidite)
                    humidite = carte[HUMIDITE][i][j+1];
                else if(carte[HUMIDITE][i][j-1] > humidite)
                    humidite = carte[HUMIDITE][i][j-1];

                if(humidite > 0)
                    carte[HUMIDITE][i][j] = humidite - 5;
            }

            if(carte[PHYSIQUE][i][j] == AIR)
                carte[HUMIDITE][i][j] = 0;
        }
    }

    return 1;
}


int pontesEtHumdite(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION], int carteNbrPopulation[TAILLE_TABLEAU_POPULATION][LARGEUR_CARTE][HAUTEUR_CARTE], int typeCouvain, int I)
{
    int humiditeMin = 0, humiditeMax = 0, nbrCasesOK = 0, caseChoisie = 0;
    int i = 0, j = 0;

    switch(typeCouvain)
    {
        case COCONS:
            humiditeMin = 10;
            humiditeMax = 39;
            break;
        case LARVES:
            humiditeMin = 70;
            humiditeMax = 99;
            break;
        case OEUFS:
            humiditeMin = 40;
            humiditeMax = 69;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
    }


    for(i = 0 ; i < LARGEUR_CARTE ; i++)
    {
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
        {
            if(carte[HUMIDITE][i][j] >= humiditeMin && carte[HUMIDITE][i][j] <= humiditeMax)
                nbrCasesOK++;

            if(carteNbrPopulation[typeCouvain][i][j] > 0)
                nbrCasesOK += 9;
        }
    }

    caseChoisie = (rand() % nbrCasesOK) + 1;

    if(caseChoisie <= 0)
        exit(EXIT_FAILURE);

    for(i = 0 ; i < LARGEUR_CARTE ; i++)
    {
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
        {
            if(carte[HUMIDITE][i][j] >= humiditeMin && carte[HUMIDITE][i][j] <= humiditeMax)
                caseChoisie--;

            if(carteNbrPopulation[typeCouvain][i][j] > 0)
                caseChoisie -= 9;

            if(caseChoisie <= 0)
            {
                couvain[typeCouvain][I].x = i;
                couvain[typeCouvain][I].y = j;
                return 1;
            }
        }
    }

    return 1;
}


int spawnReine(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], infosPopulation *reine)
{
    int nbrCasesOK = 0, caseChoisie = 0;
    int i = 0, j = 0;

    int carteCasesOK[LARGEUR_CARTE][HAUTEUR_CARTE];

    for(i = 0 ; i < LARGEUR_CARTE ; i++)
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
            carteCasesOK[i][j] = 0;

    for(i = 0 ; i < LARGEUR_CARTE ; i++)
    {
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
        {
            if((carte[PHYSIQUE][i-1][j] == TUBE ||
               (carte[PHYSIQUE][i-1][j] == VERRE && (verre(carte, i-1, j) == VERRE_GAUCHE || verre(carte, i-1, j) == VERRE_HAUT)))
               &&
               (carte[PHYSIQUE][i][j] == TUBE ||
               (carte[PHYSIQUE][i][j] == VERRE && verre(carte, i, j) == VERRE_HAUT))
               &&
               (carte[PHYSIQUE][i+1][j] == TUBE ||
               (carte[PHYSIQUE][i+1][j] == VERRE && (verre(carte, i+1, j) == VERRE_DROITE || verre(carte, i+1, j) == VERRE_HAUT)))
               &&
               (carte[PHYSIQUE][i-1][j+1] == TUBE ||
               (carte[PHYSIQUE][i-1][j+1] == VERRE && (verre(carte, i-1, j+1) == VERRE_GAUCHE || verre(carte, i-1, j+1) == VERRE_BAS)))
               &&
               (carte[PHYSIQUE][i][j+1] == TUBE ||
               (carte[PHYSIQUE][i][j+1] == VERRE && verre(carte, i, j+1) == VERRE_BAS))
               &&
               (carte[PHYSIQUE][i+1][j+1] == TUBE ||
               (carte[PHYSIQUE][i+1][j+1] == VERRE && (verre(carte, i+1, j+1) == VERRE_DROITE || verre(carte, i+1, j+1) == VERRE_BAS))))
            {
                carteCasesOK[i][j] = 1;
                nbrCasesOK++;
            }
        }
    }

    if(nbrCasesOK <= 0)
        exit(EXIT_FAILURE);

    caseChoisie = (rand() % nbrCasesOK) + 1;

    if(caseChoisie <= 0)
        exit(EXIT_FAILURE);

    for(i = 0 ; i < LARGEUR_CARTE ; i++)
    {
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
        {
            if(carteCasesOK[i][j] == 1)
                caseChoisie--;

            if(caseChoisie <= 0)
            {
                reine->x = i;
                reine->y = j;

                return 1;
            }
        }
    }

    return 1;
}
