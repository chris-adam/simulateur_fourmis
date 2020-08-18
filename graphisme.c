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

int eau(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j)
{
    //EAU_MILIEU
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_MILIEU;
    }

    //EAU_HAUT
    else if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
            (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
            (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
            (carte[PHYSIQUE][i][j-1] == AIR || carte[PHYSIQUE][i][j-1] == BORD))
    {
        return EAU_HAUT;
    }

    //EAU_BAS
    else if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
            (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
            (carte[PHYSIQUE][i][j+1] == AIR || carte[PHYSIQUE][i][j+1] == BORD) &&
            (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_BAS;
    }

    //EAU_GAUCHE
    else if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
            (carte[PHYSIQUE][i-1][j] == AIR || carte[PHYSIQUE][i-1][j] == BORD) &&
            (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
            (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_GAUCHE;
    }

    //EAU_DROITE
    else if((carte[PHYSIQUE][i+1][j] == AIR || carte[PHYSIQUE][i+1][j] == BORD) &&
            (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
            (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
            (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_DROITE;
    }

    //EAU_HAUT_GAUCHE
    else if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
            (carte[PHYSIQUE][i-1][j] == AIR || carte[PHYSIQUE][i-1][j] == BORD) &&
            (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
            (carte[PHYSIQUE][i][j-1] == AIR || carte[PHYSIQUE][i][j-1] == BORD))
    {
        return EAU_HAUT_GAUCHE;
    }

    //EAU_HAUT_DROTIE
    else if((carte[PHYSIQUE][i+1][j] == AIR || carte[PHYSIQUE][i+1][j] == BORD) &&
            (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
            (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON) &&
            (carte[PHYSIQUE][i][j-1] == AIR || carte[PHYSIQUE][i][j-1] == BORD))
    {
        return EAU_HAUT_DROITE;
    }

    //EAU_BAS_GAUCHE
    else if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON) &&
            (carte[PHYSIQUE][i-1][j] == AIR || carte[PHYSIQUE][i-1][j] == BORD) &&
            (carte[PHYSIQUE][i][j+1] == AIR || carte[PHYSIQUE][i][j+1] == BORD) &&
            (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_BAS_GAUCHE;
    }

    //EAU_BAS_DROITE
    else if((carte[PHYSIQUE][i+1][j] == AIR || carte[PHYSIQUE][i+1][j] == BORD) &&
            (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON) &&
            (carte[PHYSIQUE][i][j+1] == AIR || carte[PHYSIQUE][i][j+1] == BORD) &&
            (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON))
    {
        return EAU_BAS_DROITE;
    }

    return ERROR;
}


int coton(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j)
{
    //coton_milieu
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == TUBE) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == TUBE) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == TUBE) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == TUBE))
    {
        return COTON_MILIEU;
    }

    //coton_haut
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == VERRE) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == VERRE) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == VERRE) &&
       (carte[PHYSIQUE][i][j-1] == AIR || carte[PHYSIQUE][i][j-1] == BORD))
    {
        return COTON_HAUT;
    }

    //coton_bas
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == VERRE) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == VERRE) &&
       (carte[PHYSIQUE][i][j+1] == AIR || carte[PHYSIQUE][i][j+1] == BORD) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == VERRE))
    {
        return COTON_BAS;
    }

    //coton_gauche
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == VERRE) &&
       (carte[PHYSIQUE][i-1][j] == AIR || carte[PHYSIQUE][i-1][j] == BORD) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == VERRE) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == VERRE))
    {
        return COTON_GAUCHE;
    }

    //coton_droite
    if((carte[PHYSIQUE][i+1][j] == AIR || carte[PHYSIQUE][i+1][j] == BORD) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == VERRE) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == VERRE) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == VERRE))
    {
        return COTON_DROITE;
    }


    return ERROR;
}


int verre(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j)
{
    //verre_haut
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == VERRE || carte[PHYSIQUE][i+1][j] == AIR) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == VERRE || carte[PHYSIQUE][i-1][j] == AIR) &&
       (carte[PHYSIQUE][i][j+1] == VERRE || carte[PHYSIQUE][i][j+1] == TUBE) &&
       (carte[PHYSIQUE][i][j-1] == AIR || carte[PHYSIQUE][i][j-1] == BORD))
    {
        return VERRE_HAUT;
    }

    //verre_bas
    if((carte[PHYSIQUE][i+1][j] == EAU || carte[PHYSIQUE][i+1][j] == COTON || carte[PHYSIQUE][i+1][j] == VERRE || carte[PHYSIQUE][i+1][j] == AIR) &&
       (carte[PHYSIQUE][i-1][j] == EAU || carte[PHYSIQUE][i-1][j] == COTON || carte[PHYSIQUE][i-1][j] == VERRE || carte[PHYSIQUE][i-1][j] == AIR) &&
       (carte[PHYSIQUE][i][j+1] == AIR || carte[PHYSIQUE][i][j+1] == BORD) &&
       (carte[PHYSIQUE][i][j-1] == VERRE || carte[PHYSIQUE][i][j-1] == TUBE))
    {
        return VERRE_BAS;
    }

    //verre_gauche
    if((carte[PHYSIQUE][i+1][j] == VERRE || carte[PHYSIQUE][i+1][j] == TUBE) &&
       (carte[PHYSIQUE][i-1][j] == AIR || carte[PHYSIQUE][i-1][j] == BORD) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == VERRE || carte[PHYSIQUE][i][j+1] == AIR) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == VERRE || carte[PHYSIQUE][i][j-1] == AIR))
    {
        return VERRE_GAUCHE;
    }

    //verre_droite
    if((carte[PHYSIQUE][i+1][j] == AIR || carte[PHYSIQUE][i+1][j] == BORD) &&
       (carte[PHYSIQUE][i-1][j] == VERRE || carte[PHYSIQUE][i-1][j] == TUBE) &&
       (carte[PHYSIQUE][i][j+1] == EAU || carte[PHYSIQUE][i][j+1] == COTON || carte[PHYSIQUE][i][j+1] == VERRE || carte[PHYSIQUE][i][j+1] == AIR) &&
       (carte[PHYSIQUE][i][j-1] == EAU || carte[PHYSIQUE][i][j-1] == COTON || carte[PHYSIQUE][i][j-1] == VERRE || carte[PHYSIQUE][i][j-1] == AIR))
    {
        return VERRE_DROITE;
    }


    return ERROR;
}


int bord(int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int i, int j)
{
    //bord_haut
    if(i != 0 && i != (LARGEUR_CARTE-1) && j == 0)
    {
        return BORD_HAUT;
    }

    //bord_bas
    if(i != 0 && i != (LARGEUR_CARTE-1) && j == (HAUTEUR_CARTE-1))
    {
        return BORD_BAS;
    }

    //bord_gauche
    if(j != 0 && j != (HAUTEUR_CARTE-1) && i == 0)
    {
        return BORD_GAUCHE;
    }

    //bord_droite
    if(j != 0 && j != (HAUTEUR_CARTE-1) && i == (LARGEUR_CARTE-1))
    {
        return BORD_DROITE;
    }

    //bord_haut_gauche
    if(i == 0 && j == 0)
    {
        return BORD_HAUT_GAUCHE;
    }

    //bord_haut_droite
    if(i == (LARGEUR_CARTE-1) && j == 0)
    {
        return BORD_HAUT_DROITE;
    }

    //bord_bas_gauche
    if(i == 0 && j == (HAUTEUR_CARTE-1))
    {
        return BORD_BAS_GAUCHE;
    }

    //bord_bas_droite
    if(i == (LARGEUR_CARTE-1) && j == (HAUTEUR_CARTE-1))
    {
        return BORD_BAS_DROITE;
    }

    return ERROR;
}
