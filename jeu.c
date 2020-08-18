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


void jouer(SDL_Surface* ecran)
{
    SDL_Surface *infosSurfaces[TAILLE_TABLEAU_INFOS] = {NULL, NULL, NULL, NULL, NULL}; // surfaces pour données de la colonie
    SDL_Surface *couvainSurfaces[3] = {NULL, NULL, NULL}, *contour = NULL;
    SDL_Surface *fonds[NBR_ZOOM] = {NULL, NULL, NULL};
    SDL_Surface *quadrillages[NBR_ZOOM] = {NULL, NULL, NULL};
    SDL_Surface *ouvriereSurfaces[4] = {NULL, NULL, NULL, NULL};
    SDL_Surface *reineSurfaces[4] = {NULL, NULL, NULL, NULL};
    SDL_Surface *air[NBR_ZOOM], *tube[NBR_ZOOM], *humiditeSurface = NULL;
    SDL_Surface *eauSurfaces[NBR_ZOOM][TAILLE_TABLEAU_EAU];
    SDL_Surface *bordSurfaces[NBR_ZOOM][TAILLE_TABLEAU_BORD];
    SDL_Surface *cotonSurfaces[NBR_ZOOM][TAILLE_TABLEAU_COTON];
    SDL_Surface *verreSurfaces[NBR_ZOOM][TAILLE_TABLEAU_VERRE];
    SDL_Surface *zoomSurfaces[NBR_ZOOM] = {NULL, NULL, NULL};
    SDL_Surface *caseOK = NULL;
    SDL_Event event;
    SDL_Color couleurNoire = {0, 0, 0}, couleurGrisClair = {233, 233, 233};
    SDL_Rect pos;

    char infosTextes[TAILLE_TABLEAU_INFOS][100] = {"", "", "", "", ""}; // textes pour données de la colonie
    char humiditeTexte[4] = "", nomDuMois[4] = "";
    int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE];
    int carteNbrPopulation[TAILLE_TABLEAU_NBR][LARGEUR_CARTE][HAUTEUR_CARTE];

    infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION]; // tableau de structures pour gérer le couvain
    infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION]; // tableau de structures pour gérer la population
    infosPopulation reine;

    long nbrPopulation[TAILLE_TABLEAU_NBR] = {0, 0, 0, 0}; // nombres pour gérer l'affiche de la population/couvain
    int i = 0, j = 0, k = 0, humiditeOK = 0, grilleOK = 0, bouchonOK = 1;
    int zoom = 0, zoomPrecedent = 0, clicGaucheCarte = 0, clicGaucheZoom = 0;

    coordonnees posCarte;
    posCarte.x = 40;
    posCarte.y = 110;

    coordonnees posCarteMemoire;
    posCarteMemoire.x = 40;
    posCarteMemoire.y = 110;

    coordonnees clicGauche;
    clicGauche.x = 0;
    clicGauche.y = 0;

    int continuer = 1, nbrReines = 0, temperature = 0;
    int jour = 0, mois = 0, annee = 0; // afficher la date
    int tempsActuel = 0, tempsPrecedent = 0, tempsDepart = 0; // gestion du temps
    int jourActuel = 0, jourPrecedent = 0;

    TTF_Font *arial32 = NULL; // création police
    TTF_Font *arial8 = NULL;


    if(TTF_Init() < 0)
    {
        fprintf(stderr, "Impossible de charger la TTF : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    arial32 = TTF_OpenFont("polices/arial.ttf", 32); // création police
    arial8 = TTF_OpenFont("polices/arial.ttf", 8);

    couvainSurfaces[OEUFS] = IMG_Load("sprites/oeuf_fourmi.png");
    couvainSurfaces[LARVES] = IMG_Load("sprites/larve_fourmi.png");
    couvainSurfaces[COCONS] = IMG_Load("sprites/cocon_fourmi.png");

    ouvriereSurfaces[EST] = IMG_Load("sprites/ouvriere_fourmi_EST.png");
    ouvriereSurfaces[NORD] = IMG_Load("sprites/ouvriere_fourmi_NORD.png");
    ouvriereSurfaces[OUEST] = IMG_Load("sprites/ouvriere_fourmi_OUEST.png");
    ouvriereSurfaces[SUD] = IMG_Load("sprites/ouvriere_fourmi_SUD.png");

    reineSurfaces[EST] = IMG_Load("sprites/reine_fourmi_EST.png");
    reineSurfaces[NORD] = IMG_Load("sprites/reine_fourmi_NORD.png");
    reineSurfaces[OUEST] = IMG_Load("sprites/reine_fourmi_OUEST.png");
    reineSurfaces[SUD] = IMG_Load("sprites/reine_fourmi_SUD.png");

    air[ZOOM1] = IMG_Load("sprites/tube_a_essai/zoom1/Air1.png");
    air[ZOOM2] = IMG_Load("sprites/tube_a_essai/zoom2/Air2.png");
    air[ZOOM3] = IMG_Load("sprites/tube_a_essai/zoom3/Air3.png");

    tube[ZOOM1] = IMG_Load("sprites/tube_a_essai/zoom1/Tube1.png");
    tube[ZOOM2] = IMG_Load("sprites/tube_a_essai/zoom2/Tube2.png");
    tube[ZOOM3] = IMG_Load("sprites/tube_a_essai/zoom3/Tube3.png");

    eauSurfaces[ZOOM1][EAU_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom1/Eau1.png");
    eauSurfaces[ZOOM1][EAU_HAUT] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_haut1.png");
    eauSurfaces[ZOOM1][EAU_BAS] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_bas1.png");
    eauSurfaces[ZOOM1][EAU_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_gauche1.png");
    eauSurfaces[ZOOM1][EAU_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_droite1.png");
    eauSurfaces[ZOOM1][EAU_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_haut_gauche1.png");
    eauSurfaces[ZOOM1][EAU_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_haut_droite1.png");
    eauSurfaces[ZOOM1][EAU_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_bas_gauche1.png");
    eauSurfaces[ZOOM1][EAU_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Eau_bas_droite1.png");

    eauSurfaces[ZOOM2][EAU_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom2/Eau2.png");
    eauSurfaces[ZOOM2][EAU_HAUT] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_haut2.png");
    eauSurfaces[ZOOM2][EAU_BAS] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_bas2.png");
    eauSurfaces[ZOOM2][EAU_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_gauche2.png");
    eauSurfaces[ZOOM2][EAU_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_droite2.png");
    eauSurfaces[ZOOM2][EAU_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_haut_gauche2.png");
    eauSurfaces[ZOOM2][EAU_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_haut_droite2.png");
    eauSurfaces[ZOOM2][EAU_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_bas_gauche2.png");
    eauSurfaces[ZOOM2][EAU_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Eau_bas_droite2.png");

    eauSurfaces[ZOOM3][EAU_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom3/Eau3.png");
    eauSurfaces[ZOOM3][EAU_HAUT] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_haut3.png");
    eauSurfaces[ZOOM3][EAU_BAS] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_bas3.png");
    eauSurfaces[ZOOM3][EAU_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_gauche3.png");
    eauSurfaces[ZOOM3][EAU_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_droite3.png");
    eauSurfaces[ZOOM3][EAU_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_haut_gauche3.png");
    eauSurfaces[ZOOM3][EAU_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_haut_droite3.png");
    eauSurfaces[ZOOM3][EAU_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_bas_gauche3.png");
    eauSurfaces[ZOOM3][EAU_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Eau_bas_droite3.png");

    bordSurfaces[ZOOM1][BORD_HAUT] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_haut1.png");
    bordSurfaces[ZOOM1][BORD_BAS] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_bas1.png");
    bordSurfaces[ZOOM1][BORD_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_gauche1.png");
    bordSurfaces[ZOOM1][BORD_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_droite1.png");
    bordSurfaces[ZOOM1][BORD_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_haut_gauche1.png");
    bordSurfaces[ZOOM1][BORD_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_haut_droite1.png");
    bordSurfaces[ZOOM1][BORD_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_bas_gauche1.png");
    bordSurfaces[ZOOM1][BORD_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Bord_bas_droite1.png");

    bordSurfaces[ZOOM2][BORD_HAUT] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_haut2.png");
    bordSurfaces[ZOOM2][BORD_BAS] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_bas2.png");
    bordSurfaces[ZOOM2][BORD_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_gauche2.png");
    bordSurfaces[ZOOM2][BORD_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_droite2.png");
    bordSurfaces[ZOOM2][BORD_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_haut_gauche2.png");
    bordSurfaces[ZOOM2][BORD_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_haut_droite2.png");
    bordSurfaces[ZOOM2][BORD_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_bas_gauche2.png");
    bordSurfaces[ZOOM2][BORD_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Bord_bas_droite2.png");

    bordSurfaces[ZOOM3][BORD_HAUT] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_haut3.png");
    bordSurfaces[ZOOM3][BORD_BAS] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_bas3.png");
    bordSurfaces[ZOOM3][BORD_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_gauche3.png");
    bordSurfaces[ZOOM3][BORD_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_droite3.png");
    bordSurfaces[ZOOM3][BORD_HAUT_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_haut_gauche3.png");
    bordSurfaces[ZOOM3][BORD_HAUT_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_haut_droite3.png");
    bordSurfaces[ZOOM3][BORD_BAS_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_bas_gauche3.png");
    bordSurfaces[ZOOM3][BORD_BAS_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Bord_bas_droite3.png");

    cotonSurfaces[ZOOM1][COTON_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom1/Coton1.png");
    cotonSurfaces[ZOOM1][COTON_HAUT] = IMG_Load("sprites/tube_a_essai/zoom1/Coton_haut1.png");
    cotonSurfaces[ZOOM1][COTON_BAS] = IMG_Load("sprites/tube_a_essai/zoom1/Coton_bas1.png");
    cotonSurfaces[ZOOM1][COTON_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Coton_gauche1.png");
    cotonSurfaces[ZOOM1][COTON_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Coton_droite1.png");

    cotonSurfaces[ZOOM2][COTON_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom2/Coton2.png");
    cotonSurfaces[ZOOM2][COTON_HAUT] = IMG_Load("sprites/tube_a_essai/zoom2/Coton_haut2.png");
    cotonSurfaces[ZOOM2][COTON_BAS] = IMG_Load("sprites/tube_a_essai/zoom2/Coton_bas2.png");
    cotonSurfaces[ZOOM2][COTON_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Coton_gauche2.png");
    cotonSurfaces[ZOOM2][COTON_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Coton_droite2.png");

    cotonSurfaces[ZOOM3][COTON_MILIEU] = IMG_Load("sprites/tube_a_essai/zoom3/Coton3.png");
    cotonSurfaces[ZOOM3][COTON_HAUT] = IMG_Load("sprites/tube_a_essai/zoom3/Coton_haut3.png");
    cotonSurfaces[ZOOM3][COTON_BAS] = IMG_Load("sprites/tube_a_essai/zoom3/Coton_bas3.png");
    cotonSurfaces[ZOOM3][COTON_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Coton_gauche3.png");
    cotonSurfaces[ZOOM3][COTON_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Coton_droite3.png");

    verreSurfaces[ZOOM1][VERRE_HAUT] = IMG_Load("sprites/tube_a_essai/zoom1/Verre_haut1.png");
    verreSurfaces[ZOOM1][VERRE_BAS] = IMG_Load("sprites/tube_a_essai/zoom1/Verre_bas1.png");
    verreSurfaces[ZOOM1][VERRE_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom1/Verre_gauche1.png");
    verreSurfaces[ZOOM1][VERRE_DROITE] = IMG_Load("sprites/tube_a_essai/zoom1/Verre_droite1.png");

    verreSurfaces[ZOOM2][VERRE_HAUT] = IMG_Load("sprites/tube_a_essai/zoom2/Verre_haut2.png");
    verreSurfaces[ZOOM2][VERRE_BAS] = IMG_Load("sprites/tube_a_essai/zoom2/Verre_bas2.png");
    verreSurfaces[ZOOM2][VERRE_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom2/Verre_gauche2.png");
    verreSurfaces[ZOOM2][VERRE_DROITE] = IMG_Load("sprites/tube_a_essai/zoom2/Verre_droite2.png");

    verreSurfaces[ZOOM3][VERRE_HAUT] = IMG_Load("sprites/tube_a_essai/zoom3/Verre_haut3.png");
    verreSurfaces[ZOOM3][VERRE_BAS] = IMG_Load("sprites/tube_a_essai/zoom3/Verre_bas3.png");
    verreSurfaces[ZOOM3][VERRE_GAUCHE] = IMG_Load("sprites/tube_a_essai/zoom3/Verre_gauche3.png");
    verreSurfaces[ZOOM3][VERRE_DROITE] = IMG_Load("sprites/tube_a_essai/zoom3/Verre_droite3.png");

    contour = IMG_Load("sprites/contour.png");

    caseOK = IMG_Load("sprites/caseOK.png");

    fonds[ZOOM1] = IMG_Load("sprites/fond1.png");
    fonds[ZOOM2] = IMG_Load("sprites/fond2.png");
    fonds[ZOOM3] = IMG_Load("sprites/fond3.png");

    quadrillages[ZOOM1] = IMG_Load("sprites/quadrillage1.png");
    quadrillages[ZOOM2] = IMG_Load("sprites/quadrillage2.png");
    quadrillages[ZOOM3] = IMG_Load("sprites/quadrillage3.png");

    zoomSurfaces[ZOOM1] = IMG_Load("sprites/Zoom_1.png");
    zoomSurfaces[ZOOM2] = IMG_Load("sprites/Zoom_2.png");
    zoomSurfaces[ZOOM3] = IMG_Load("sprites/Zoom_3.png");


    // initialisation carte physique
    for(i = 0 ; i < LARGEUR_CARTE ; i++)
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
            carte[PHYSIQUE][i][j] = 0;

    if(!chargerCarte(carte, 1))
        exit(EXIT_FAILURE);


    // initialisation carte humidité
    for(i = 0 ; i < LARGEUR_CARTE ; i++)
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
            carte[HUMIDITE][i][j] = 0;

    if(!humidite(carte))
        exit(EXIT_FAILURE);

    // initialisation carte Nbr Population
    for(i = 0 ; i < LARGEUR_CARTE ; i++)
        for(j = 0 ; j < HAUTEUR_CARTE ; j++)
            for(k = 0 ; k < TAILLE_TABLEAU_POPULATION ; k++)
                carteNbrPopulation[k][i][j] = 0;


    srand(time(NULL));


    // initialisation infos de la population
    for(i = 0 ; i < TAILLE_TABLEAU_POPULATION ; i++)
    {
        for(j = 0 ; j < MAX_POPULATION ; j++)
        {
            population[i][j].x = 0;
            population[i][j].y = 0;

            population[i][j].vie = 0;

            population[i][j].directionPrecedente = UNDEFINED;
            population[i][j].direction = UNDEFINED;
            population[i][j].delaiAnimation = 0;

            population[i][j].comportement = UNDEFINED;
        }
    }


    // initialisation infos du couvain
    for(i = 0 ; i < TAILLE_TABLEAU_COUVAIN ; i++)
    {
        for(j = 0 ; j < MAX_POPULATION ; j++)
        {
            couvain[i][j].x = 0;
            couvain[i][j].y = 0;

            couvain[i][j].delai = 0;
        }
    }


    // initialisation infos reine
    reine.x = 0;
    reine.y = 0;

    reine.vie = TEMPS_REINES;

    reine.directionPrecedente = UNDEFINED;
    reine.direction = UNDEFINED;
    reine.delaiAnimation = 0;

    reine.comportement = UNDEFINED;

    // calcul des coordonnées de la reine
    if(!spawnReine(carte, &reine))
        exit(EXIT_FAILURE);

    // initialisation temps de lancement de la colonie
    tempsDepart = SDL_GetTicks();

    // Initialisation des positions des surfaces inutiles
    reinitialisationSurfacesInutiles(population, couvain);


    while(continuer)
    {
        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_1:
                        zoom = 0;
                        posCarteMemoire.x = (posCarteMemoire.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarteMemoire.y = (posCarteMemoire.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        posCarte.x = (posCarte.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarte.y = (posCarte.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        zoomPrecedent = zoom;
                        break;
                    case SDLK_2:
                        zoom = 1;
                        posCarteMemoire.x = (posCarteMemoire.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarteMemoire.y = (posCarteMemoire.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        posCarte.x = (posCarte.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarte.y = (posCarte.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        zoomPrecedent = zoom;
                        break;
                    case SDLK_3:
                        zoom = 2;
                        posCarteMemoire.x = (posCarteMemoire.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarteMemoire.y = (posCarteMemoire.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        posCarte.x = (posCarte.x - 640 ) / pow(2, zoomPrecedent - zoom) + 640;
                        posCarte.y = (posCarte.y - 360 ) / pow(2, zoomPrecedent - zoom) + 360;
                        zoomPrecedent = zoom;
                        break;
                    case SDLK_g:
                        if(!grilleOK)
                            grilleOK = 1;
                        else if(grilleOK)
                            grilleOK = 0;
                        break;
                    case SDLK_z:
                        if(!humiditeOK)
                            humiditeOK = 1;
                        else if(humiditeOK)
                            humiditeOK = 0;
                        break;
                    case SDLK_p:
                        if(!bouchonOK)
                        {
                            bouchonOK = 1;
                            if(!chargerCarte(carte, 1))
                                exit(EXIT_FAILURE);
                        }
                        else if(bouchonOK)
                        {
                            bouchonOK = 0;
                            if(!chargerCarte(carte, 2))
                                exit(EXIT_FAILURE);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        clicGauche.x = event.button.x;
                        clicGauche.y = event.button.y;

                        if(event.button.x >= 40 && event.button.x <= 1240 &&
                           event.button.y >= 110 && event.button.y <= 610)
                            clicGaucheCarte = 1;

                        if(event.button.x >= 150 && event.button.x <= 481 &&
                           event.button.y >= 665 && event.button.y <= 707)
                            clicGaucheZoom = 1;

                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                if(clicGaucheCarte)
                {
                    posCarte.x = posCarteMemoire.x + event.motion.x - clicGauche.x;
                    posCarte.y = posCarteMemoire.y + event.motion.y - clicGauche.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        posCarteMemoire.x = posCarte.x;
                        posCarteMemoire.y = posCarte.y;
                        clicGauche.x = 0;
                        clicGauche.y = 0;
                        if(clicGaucheCarte)
                            clicGaucheCarte = 0;
                        if(event.button.x > 43 && event.button.x < 80 &&
                           event.button.y > 618 && event.button.y < 655)
                        {
                            if(!grilleOK)
                                grilleOK = 1;
                            else if(grilleOK)
                                grilleOK = 0;
                        }
                        if(event.button.x > 192 && event.button.x < 228 &&
                           event.button.y > 618 && event.button.y < 655)
                        {
                            if(!humiditeOK)
                                humiditeOK = 1;
                            else if(humiditeOK)
                                humiditeOK = 0;
                        }
                        if(event.button.x > 341 && event.button.x < 378 &&
                           event.button.y > 618 && event.button.y < 655)
                        {
                            if(!bouchonOK)
                            {
                                bouchonOK = 1;
                                if(!chargerCarte(carte, 1))
                                    exit(EXIT_FAILURE);
                            }
                            else if(bouchonOK)
                            {
                                bouchonOK = 0;
                                if(!chargerCarte(carte, 2))
                                    exit(EXIT_FAILURE);
                            }
                        }

                        if(clicGaucheZoom)
                        {
                            if(event.button.x >= 155 && event.button.x <= 241 &&
                               event.button.y >= 665 && event.button.y <= 707)
                                zoom = 0;
                            else if(event.button.x >= 242 && event.button.x <= 387 &&
                               event.button.y >= 665 && event.button.y <= 707)
                                zoom = 1;
                            else if(event.button.x >= 388 && event.button.x <= 475 &&
                               event.button.y >= 665 && event.button.y <= 707)
                                zoom = 2;

                            clicGaucheZoom = 0;
                        }

                        break;
                    case SDL_BUTTON_RIGHT:
                        posCarteMemoire.x = 40;
                        posCarteMemoire.y = 110;
                        posCarte.x = 40;
                        posCarte.y = 110;
                        break;
                    case SDL_BUTTON_WHEELUP:
                        if(zoom < 2)
                        {
                            zoom++;
                            posCarteMemoire.x = (posCarteMemoire.x - 640 ) * 2 + 640;
                            posCarteMemoire.y = (posCarteMemoire.y - 360 ) * 2 + 360;
                            posCarte.x = (posCarte.x - 640 ) * 2 + 640;
                            posCarte.y = (posCarte.y - 360 ) * 2 + 360;
                            zoomPrecedent = zoom;
                        }
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        if(zoom > 0)
                        {
                            zoom--;
                            posCarteMemoire.x = (posCarteMemoire.x - 640 ) / 2 + 640;
                            posCarteMemoire.y = (posCarteMemoire.y - 360 ) / 2 + 360;
                            posCarte.x = (posCarte.x - 640 ) / 2 + 640;
                            posCarte.y = (posCarte.y - 360 ) / 2 + 360;
                            zoomPrecedent = zoom;
                        }
                        break;
                    default:
                        break;
                }
                event.button.x = 0;
                event.button.y = 0;
                break;
            default:
                break;
        }

        // on mets le jeu à jour régulièrement
        tempsActuel = (SDL_GetTicks() - tempsDepart);
        if((tempsActuel-tempsPrecedent) >= DELAY_MAJ)
        {

            tempsPrecedent = (SDL_GetTicks() - tempsDepart);

            tempsActuel = ((SDL_GetTicks() - tempsDepart) / VITESSE_JEU);

            // MAJ de la date
            annee = (tempsActuel / 360) + 2000;
            mois = ((tempsActuel % 360) / 30) + 1;
            jour = ((tempsActuel % 360) % 30) + 1;


            jourActuel = jour + (mois-1) * 30 + (annee-2000) * 360;

            if(jourActuel > jourPrecedent)
            {
                // gestion de la température en fonction de la période de l'année
                temperature = 0;
                temperature = meteo(mois);
                if(temperature == 0)
                {
                    fprintf(stderr, "Impossible de calculer la température");
                    exit(EXIT_FAILURE);
                }

                // évolution du couvain
                gestionPop(population, couvain, carteNbrPopulation, &reine, carte, temperature);

                // gestion des pontes
                gestionPontes(couvain, carte, carteNbrPopulation, nbrPopulation, nbrReines, temperature);

                // initialisation population/couvain
                for(i = 0 ; i < TAILLE_TABLEAU_NBR ; i++)
                    nbrPopulation[i] = 0;
                nbrReines = 0;

                // remplissage nbrPopulation
                for(i = 0 ; i < MAX_POPULATION ; i++)
                {
                    if(couvain[OEUFS][i].delai > 0)
                        nbrPopulation[NBR_OEUFS]++;

                    if(couvain[LARVES][i].delai > 0)
                        nbrPopulation[NBR_LARVES]++;

                    if(couvain[COCONS][i].delai > 0)
                        nbrPopulation[NBR_COCONS]++;

                    if(population[OUVRIERES][i].vie > 0)
                        nbrPopulation[NBR_OUVRIERES]++;
                }

                    if(reine.vie > 0)
                        nbrReines++;


                // reinitialisation carte Nbr Population
                for(i = 0 ; i < LARGEUR_CARTE ; i++)
                    for(j = 0 ; j < HAUTEUR_CARTE ; j++)
                        for(k = 0 ; k < TAILLE_TABLEAU_NBR ; k++)
                            carteNbrPopulation[k][i][j] = 0;

                // remplissage carteNbrPopulation
                for(i = 0 ; i < TAILLE_TABLEAU_NBR ; i++)
                    for(j = 0 ; j < MAX_POPULATION ; j++)
                        switch(i)
                        {
                            case NBR_OUVRIERES:
                                if(population[OUVRIERES][j].vie > 0)
                                    carteNbrPopulation[i][population[OUVRIERES][j].x][population[OUVRIERES][j].y]++;
                                break;

                            case NBR_OEUFS: case NBR_LARVES: case NBR_COCONS:
                                if(couvain[i][j].delai > 0)
                                    carteNbrPopulation[i][couvain[i][j].x][couvain[i][j].y]++;
                                break;
                        }

                jourPrecedent = jourActuel;
            }

            // Gestion déplacement des ouvrières
            deplacementOuvrieres(population, carte);

            // Gestion déplacement de la reine
            deplacementReine(&reine, carte);

            // on efface l'ecran
            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

            // on écrit les données dans des chaînes
            sprintf(infosTextes[EGGS], "%ld", nbrPopulation[NBR_OEUFS]);
            sprintf(infosTextes[LARVAE], "%ld", nbrPopulation[NBR_LARVES]);
            sprintf(infosTextes[NYMPHS], "%ld", nbrPopulation[NBR_COCONS]);
            sprintf(infosTextes[QUEEN], "%d", nbrReines);
            sprintf(infosTextes[WORKERS], "%ld", nbrPopulation[NBR_OUVRIERES]);
            sprintf(infosTextes[TEMPS], "%d %s %d", jour, nomMois(mois, nomDuMois), annee);
            sprintf(infosTextes[JOUR], "%d", jourActuel);
            sprintf(infosTextes[TEMPERATURE], "%d", temperature);

            // on place les chaîne dans des surfaces avec SDL_ttf
            infosSurfaces[EGGS] = TTF_RenderText_Shaded(arial32, infosTextes[EGGS], couleurNoire, couleurGrisClair);
            infosSurfaces[LARVAE] = TTF_RenderText_Shaded(arial32, infosTextes[LARVAE], couleurNoire, couleurGrisClair);
            infosSurfaces[NYMPHS] = TTF_RenderText_Shaded(arial32, infosTextes[NYMPHS], couleurNoire, couleurGrisClair);
            infosSurfaces[QUEEN] = TTF_RenderText_Shaded(arial32, infosTextes[QUEEN], couleurNoire, couleurGrisClair);
            infosSurfaces[WORKERS] = TTF_RenderText_Shaded(arial32, infosTextes[WORKERS], couleurNoire, couleurGrisClair);
            infosSurfaces[TEMPS] = TTF_RenderText_Shaded(arial32, infosTextes[TEMPS], couleurNoire, couleurGrisClair);
            infosSurfaces[JOUR] = TTF_RenderText_Shaded(arial32, infosTextes[JOUR], couleurNoire, couleurGrisClair);
            infosSurfaces[TEMPERATURE] = TTF_RenderText_Shaded(arial32, infosTextes[TEMPERATURE], couleurNoire, couleurGrisClair);

            //on colle le fond de cases rouges
            pos.x = posCarte.x;
            pos.y = posCarte.y;
            SDL_BlitSurface(fonds[zoom], NULL, ecran, &pos);

            //on colle la carte
            for(i = 0 ; i < LARGEUR_CARTE ; i++)
            {
                for(j = 0 ; j < HAUTEUR_CARTE ; j++)
                {
                    pos.x = posCarte.x + i*20*pow(2, zoom);
                    pos.y = posCarte.y + j*20*pow(2, zoom);

                    switch(carte[PHYSIQUE][i][j])
                    {
                        case AIR:
                            SDL_BlitSurface(air[zoom], NULL, ecran, &pos);
                            break;
                        case EAU:
                            SDL_BlitSurface(eauSurfaces[zoom][eau(carte, i, j)], NULL, ecran, &pos);
                            break;
                        case COTON:
                            SDL_BlitSurface(cotonSurfaces[zoom][coton(carte, i, j)], NULL, ecran, &pos);
                            break;
                        case VERRE:
                            SDL_BlitSurface(verreSurfaces[zoom][verre(carte, i, j)], NULL, ecran, &pos);
                            break;
                        case TUBE:
                            SDL_BlitSurface(tube[zoom], NULL, ecran, &pos);
                            break;
                        case BORD:
                            SDL_BlitSurface(bordSurfaces[zoom][bord(carte, i, j)], NULL, ecran, &pos);
                            break;
                        default:
                            break;
                    }
                }
            }

            // on colle le couvain, les ouvrières et la reine
            for(i = 0 ; i < LARGEUR_CARTE ; i++)
                for(j = 0 ; j < HAUTEUR_CARTE ; j++)
                {
                    pos.x = posCarte.x + i*20*pow(2, zoom);
                    pos.y = posCarte.y + j*20*pow(2, zoom);

                    if(carteNbrPopulation[NBR_OEUFS][i][j] > 0)
                        SDL_BlitSurface(couvainSurfaces[OEUFS], NULL, ecran, &pos);

                    if(carteNbrPopulation[NBR_LARVES][i][j] > 0)
                        SDL_BlitSurface(couvainSurfaces[LARVES], NULL, ecran, &pos);

                    if(carteNbrPopulation[NBR_COCONS][i][j] > 0)
                        SDL_BlitSurface(couvainSurfaces[COCONS], NULL, ecran, &pos);
                }

            for(i = 0 ; i < MAX_POPULATION ; i++)
            {
                if(population[OUVRIERES][i].vie > 0)
                {
                    pos.x = posCarte.x + population[OUVRIERES][i].x*20*pow(2, zoom);
                    pos.y = posCarte.y + population[OUVRIERES][i].y*20*pow(2, zoom);

                    if(population[OUVRIERES][i].direction == EST)
                        pos.x -= 20*pow(2, zoom);
                    else if(population[OUVRIERES][i].direction == SUD)
                        pos.y -= 20*pow(2, zoom);

                    SDL_BlitSurface(ouvriereSurfaces[population[OUVRIERES][i].direction], NULL, ecran, &pos);
                }
            }

            if(reine.vie > 0)
            {
                pos.x = posCarte.x + reine.x*20*pow(2, zoom);
                pos.y = posCarte.y + reine.y*20*pow(2, zoom);

                if(reine.direction == EST)
                    pos.x -= 20*pow(2, zoom);
                else if(reine.direction == SUD)
                    pos.y -= 20*pow(2, zoom);

                SDL_BlitSurface(reineSurfaces[reine.direction], NULL, ecran, &pos);
            }

            //on colle l'humidite
            if(humiditeOK)
            {
                for(i = 0 ; i < LARGEUR_CARTE ; i++)
                {
                    for(j = 0 ; j < HAUTEUR_CARTE ; j++)
                    {
                        pos.x = posCarte.x + i*20*pow(2, zoom);
                        pos.y = posCarte.y + j*20*pow(2, zoom);

                        sprintf(humiditeTexte, "%d", carte[HUMIDITE][i][j]);

                        humiditeSurface = TTF_RenderText_Solid(arial8, humiditeTexte, couleurNoire);

                        SDL_BlitSurface(humiditeSurface, NULL, ecran, &pos);

                        SDL_FreeSurface(humiditeSurface);
                    }
                }
            }


            //on colle le quadrillage
            if(grilleOK)
            {
                pos.x = posCarte.x;
                pos.y = posCarte.y;
                SDL_BlitSurface(quadrillages[zoom], NULL, ecran, &pos);
            }

            //on colle le contour
            pos.x = 0;
            pos.y = 0;
            SDL_BlitSurface(contour, NULL, ecran, &pos);

            // on colle les "V" dans les cases
            if(grilleOK)
            {
                pos.x = 39;
                pos.y = 614;

                SDL_BlitSurface(caseOK, NULL, ecran, &pos);
            }

            if(humiditeOK)
            {
                pos.x = 188;
                pos.y = 614;

                SDL_BlitSurface(caseOK, NULL, ecran, &pos);
            }

            if(bouchonOK)
            {
                pos.x = 337;
                pos.y = 614;

                SDL_BlitSurface(caseOK, NULL, ecran, &pos);
            }

            // on colle le marqueur du zoom
            if(!clicGaucheZoom)
            {
                pos.x = 150;
                pos.y = 665;
                SDL_BlitSurface(zoomSurfaces[zoom], NULL, ecran, &pos);
            }

            else if(clicGaucheZoom)
            {
                if(event.motion.x >= 149 && event.motion.x <= 438)
                    pos.x = event.motion.x - 21;
                else if(event.motion.x < 149)
                    pos.x = 150;
                else if(event.motion.x > 438)
                    pos.x = 438;
                pos.y = 665;
                SDL_BlitSurface(zoomSurfaces[0], NULL, ecran, &pos);
            }

            // on colle les cinq surfaces de textes
            pos.x = 534;
            pos.y = 9;
            SDL_BlitSurface(infosSurfaces[EGGS], NULL, ecran, &pos);

            pos.x = 846;
            pos.y = 9;
            SDL_BlitSurface(infosSurfaces[LARVAE], NULL, ecran, &pos);

            pos.x = 1174;
            pos.y = 9;
            SDL_BlitSurface(infosSurfaces[NYMPHS], NULL, ecran, &pos);

            pos.x = 943;
            pos.y = 62;
            SDL_BlitSurface(infosSurfaces[QUEEN], NULL, ecran, &pos);

            pos.x = 1174;
            pos.y = 62;
            SDL_BlitSurface(infosSurfaces[WORKERS], NULL, ecran, &pos);

            pos.x = 103;
            pos.y = 9;
            SDL_BlitSurface(infosSurfaces[TEMPS], NULL, ecran, &pos);

            pos.x = 106;
            pos.y = 62;
            SDL_BlitSurface(infosSurfaces[JOUR], NULL, ecran, &pos);

            pos.x = 626;
            pos.y = 62;
            SDL_BlitSurface(infosSurfaces[TEMPERATURE], NULL, ecran, &pos);

            // on mets à jour l'ecran
            SDL_Flip(ecran);


            for(i = 0 ; i < TAILLE_TABLEAU_INFOS ; i++)
            {
                SDL_FreeSurface(infosSurfaces[i]);
            }
        }

        // si il est trop tôt, on attends
        else if((tempsActuel-tempsPrecedent) < DELAY_MAJ)
        {
            SDL_Delay(DELAY_MAJ - (tempsActuel-tempsPrecedent));
        }

    }

    TTF_CloseFont(arial32);
    TTF_CloseFont(arial8);
    TTF_Quit();

    for(i = 0 ; i < 3 ; i++)
        SDL_FreeSurface(couvainSurfaces[i]);

    for(i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(ouvriereSurfaces[i]);

    for(i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(reineSurfaces[i]);

    for(i = 0 ; i < NBR_ZOOM ; i++)
        SDL_FreeSurface(air[i]);

    for(i = 0 ; i < NBR_ZOOM ; i++)
        SDL_FreeSurface(tube[i]);

    for(i = 0 ; i < TAILLE_TABLEAU_EAU ; i++)
        for(j = 0 ; j < NBR_ZOOM ; j++)
            SDL_FreeSurface(eauSurfaces[j][i]);

    for(i = 0 ; i < TAILLE_TABLEAU_COTON ; i++)
        for(j = 0 ; j < NBR_ZOOM ; j++)
            SDL_FreeSurface(cotonSurfaces[j][i]);

    for(i = 0 ; i < TAILLE_TABLEAU_VERRE ; i++)
        for(j = 0 ; j < NBR_ZOOM ; j++)
            SDL_FreeSurface(verreSurfaces[j][i]);

    for(i = 0 ; i < NBR_ZOOM ; i++)
        SDL_FreeSurface(zoomSurfaces[i]);

    SDL_FreeSurface(contour);

    for(i = 0 ; i < 3 ; i++)
        SDL_FreeSurface(fonds[i]);

    for(i = 0 ; i < 3 ; i++)
        SDL_FreeSurface(quadrillages[i]);
}


void gestionPop(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION], int carteNbrPopulation[TAILLE_TABLEAU_POPULATION][LARGEUR_CARTE][HAUTEUR_CARTE], infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int temperature)
{
    int i = 0, j = 0, transformation = 1;


    if(temperature < 15)
        temperature = 15;
    else if(temperature > 25)
        temperature = 25;

    temperature -= 15;

    // on gère l'évolution du couvain
    for(i = 0 ; i < MAX_POPULATION ; i++)
    {
        if(population[OUVRIERES][i].vie > 0)
        {
            population[OUVRIERES][i].vie -= temperature;
        }

        if(couvain[COCONS][i].delai > 0)
        {
            couvain[COCONS][i].delai -= (temperature + 1);

            if(couvain[COCONS][i].delai <= 0)
            {
                j = 0;
                while(transformation && j < MAX_POPULATION)
                {
                    if(population[OUVRIERES][j].vie <= 0)
                    {
                        population[OUVRIERES][j].vie = TEMPS_OUVRIERES;
                        population[OUVRIERES][j].x = couvain[COCONS][i].x;
                        population[OUVRIERES][j].y = couvain[COCONS][i].y;
                        transformation = 0;
                    }
                    j++;
                }

                if(transformation)
                {
                    population[OUVRIERES][i].vie = TEMPS_OUVRIERES;
                    population[OUVRIERES][i].x = couvain[COCONS][i].x;
                    population[OUVRIERES][i].y = couvain[COCONS][i].y;
                }

                transformation = 1;
            }
        }

        if(couvain[LARVES][i].delai > 0)
        {
            couvain[LARVES][i].delai -= temperature;

            if(couvain[LARVES][i].delai <= 0)
            {
                j = 0;
                while(transformation && j < MAX_POPULATION)
                {
                    if(couvain[COCONS][j].delai <= 0)
                    {
                        couvain[COCONS][j].delai = TEMPS_COCONS;
                        if(!pontesEtHumdite(carte, couvain, carteNbrPopulation, COCONS, j))
                            exit(EXIT_FAILURE);
                        transformation = 0;
                    }
                    j++;
                }

                if(transformation)
                {
                    couvain[COCONS][i].delai = TEMPS_COCONS;
                    if(!pontesEtHumdite(carte, couvain, carteNbrPopulation, COCONS, i))
                        exit(EXIT_FAILURE);
                }

                transformation = 1;
            }
        }

        if(couvain[OEUFS][i].delai > 0)
        {
            couvain[OEUFS][i].delai -= (temperature + 1);

            if(couvain[OEUFS][i].delai <= 0)
            {
                j = 0;
                while(transformation && j < MAX_POPULATION)
                {
                    if(couvain[LARVES][j].delai <= 0)
                    {
                        couvain[LARVES][j].delai = TEMPS_LARVES;

                        if(!pontesEtHumdite(carte, couvain, carteNbrPopulation, LARVES, j))
                            exit(EXIT_FAILURE);

                        transformation = 0;
                    }
                    j++;
                }

                if(transformation)
                {
                    couvain[LARVES][i].delai = TEMPS_LARVES;
                    if(!pontesEtHumdite(carte, couvain, carteNbrPopulation, LARVES, i))
                        exit(EXIT_FAILURE);
                }

                transformation = 1;
            }
        }
    }

    if(reine->vie >= 1)
        reine->vie = reine->vie - temperature - 1;

}


void gestionPontes(infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int carteNbrPopulation[TAILLE_TABLEAU_POPULATION][LARGEUR_CARTE][HAUTEUR_CARTE], long nbrPopulation[], int nbrReines, int temperature)
{
    int i = 0, nbrOeufsAPondre = 0, pontesMax = 0, couvainMax = 0, ecrasementParabole = 0;


    if(temperature < 15)
        temperature = 15;
    else if(temperature > 25)
        temperature = 25;

    pontesMax = (temperature-15) * 3;

    if(pontesMax > 0)
    {
        if(nbrPopulation[NBR_OUVRIERES] < 1)
            nbrPopulation[NBR_OUVRIERES] = 1;

        couvainMax = (rand() % (5*nbrPopulation[NBR_OUVRIERES] - nbrPopulation[NBR_OUVRIERES] + 1)) + nbrPopulation[NBR_OUVRIERES];

        if(couvainMax >= MAX_POPULATION)
            couvainMax = MAX_POPULATION;

        ecrasementParabole = pow(couvainMax, 2) / pontesMax;

        nbrOeufsAPondre = pow(((nbrPopulation[OEUFS]/2 + nbrPopulation[LARVES] + nbrPopulation[COCONS]/3) - couvainMax), 2) / ecrasementParabole;
        nbrOeufsAPondre *= nbrReines;
    }

    if(nbrOeufsAPondre <= 0)
        nbrOeufsAPondre = 0;
    else if((nbrPopulation[OEUFS]/3 + nbrPopulation[LARVES] + nbrPopulation[COCONS]/5) >= couvainMax)
        nbrOeufsAPondre = couvainMax - (nbrPopulation[OEUFS]/2 + nbrPopulation[LARVES] + nbrPopulation[COCONS]/3);


    if((nbrOeufsAPondre + nbrPopulation[OEUFS]) <= MAX_POPULATION)
    {
        while(nbrOeufsAPondre > 0)
        {
            if(couvain[OEUFS][i].delai <= 0)
            {
                couvain[OEUFS][i].delai = TEMPS_OEUFS;
                pontesEtHumdite(carte, couvain, carteNbrPopulation, OEUFS, i);
                nbrOeufsAPondre--;
            }

            i++;
        }
    }

    else if((nbrOeufsAPondre + nbrPopulation[OEUFS]) > MAX_POPULATION)
    {
        for(i = 0 ; i < MAX_POPULATION ; i++)
        {

            if(couvain[OEUFS][i].delai <= 0)
            {
                couvain[OEUFS][i].delai = TEMPS_OEUFS;
                if(!pontesEtHumdite(carte, couvain, carteNbrPopulation, OEUFS, i));
                    exit(EXIT_FAILURE);
            }
        }
    }
}


int meteo(int mois)
{
    int temperature = 0, MIN = 0, MAX = 0;


    switch(mois)
    {
        case 1:
            MIN = 3;
            MAX = 7;
            break;
        case 2:
            MIN = 3;
            MAX = 8;
            break;
        case 3:
            MIN = 5;
            MAX = 12;
            break;
        case 4:
            MIN = 7;
            MAX = 16;
            break;
        case 5:
            MIN = 11;
            MAX = 20;
            break;
        case 6:
            MIN = 14;
            MAX = 23;
            break;
        case 7:
            MIN = 16;
            MAX = 25;
            break;
        case 8:
            MIN = 16;
            MAX = 25;
            break;
        case 9:
            MIN = 13;
            MAX = 21;
            break;
        case 10:
            MIN = 10;
            MAX = 16;
            break;
        case 11:
            MIN = 6;
            MAX = 11;
            break;
        case 12:
            MIN = 3;
            MAX = 8;
            break;
        default:
            MIN = 15;
            MAX = 25;
    }

    if(MIN != 0 && MAX != 0)
    {
        temperature = (rand() % (MAX - MIN + 1)) + MIN;
    }


    return temperature;
}


void reinitialisationSurfacesInutiles(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION])
{
    int i = 0;

    for(i = 0 ; i < MAX_POPULATION ; i++)
    {
        if(couvain[OEUFS][i].delai <= 0)
        {
            couvain[OEUFS][i].x = 0;
            couvain[OEUFS][i].y = 0;

            couvain[OEUFS][i].delai = 0;
        }

        if(couvain[LARVES][i].delai <= 0)
        {
            couvain[LARVES][i].x = 0;
            couvain[LARVES][i].y = 0;

            couvain[LARVES][i].delai = 0;
        }

        if(couvain[COCONS][i].delai <= 0)
        {
            couvain[COCONS][i].x = 0;
            couvain[COCONS][i].y = 0;

            couvain[COCONS][i].delai = 0;
        }

        if(population[OUVRIERES][i].vie <= 0)
        {
            population[OUVRIERES][i].x = 0;
            population[OUVRIERES][i].y = 0;

            population[OUVRIERES][i].vie = 0;

            population[OUVRIERES][i].directionPrecedente = UNDEFINED;
            population[OUVRIERES][i].direction = UNDEFINED;
            population[OUVRIERES][i].delaiAnimation = 0;

            population[OUVRIERES][i].comportement = UNDEFINED;
        }
    }
}


void deplacementOuvrieres(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE])
{
    int i = 0;

    for(i = 0 ; i < MAX_POPULATION ; i++)
    {
        if(population[OUVRIERES][i].vie > 0 && population[OUVRIERES][i].delaiAnimation <= 0)
        {
            population[OUVRIERES][i].directionPrecedente = population[OUVRIERES][i].direction;

            do
            {
                population[OUVRIERES][i].direction += ((rand() % 3) - 1);

                if(population[OUVRIERES][i].direction < 0)
                {
                    population[OUVRIERES][i].direction += 4;
                }
                else if(population[OUVRIERES][i].direction > 3)
                {
                    population[OUVRIERES][i].direction -= 4;
                }
            } while(gestionCollisionOuvriere(population, carte, i));

            switch(population[OUVRIERES][i].direction)
            {
                case EST:
                    population[OUVRIERES][i].x++;
                    break;
                case NORD:
                    population[OUVRIERES][i].y--;
                    break;
                case OUEST:
                    population[OUVRIERES][i].x--;
                    break;
                case SUD:
                    population[OUVRIERES][i].y++;
                    break;
            }

            population[OUVRIERES][i].delaiAnimation = 5;
        }

        else if(population[OUVRIERES][i].delaiAnimation > 0)
            population[OUVRIERES][i].delaiAnimation--;
    }
}


int gestionCollisionOuvriere(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int I)
{
    switch(population[OUVRIERES][I].direction)
    {
        case EST:
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == BORD &&
                bord(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == BORD_DROITE)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == VERRE &&
                verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == VERRE_DROITE)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x + 1][population[OUVRIERES][I].y] == COTON ||
                carte[PHYSIQUE][population[OUVRIERES][I].x + 1][population[OUVRIERES][I].y] == EAU)
                return 1;
            else if(carte[PHYSIQUE][population[OUVRIERES][I].x + 1][population[OUVRIERES][I].y] == VERRE &&
                    verre(carte, population[OUVRIERES][I].x + 1, population[OUVRIERES][I].y) == VERRE_GAUCHE)
                return 1;
            break;

        case SUD:
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == BORD &&
                bord(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == BORD_BAS)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == VERRE &&
                verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == VERRE_BAS)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y + 1] == COTON ||
                carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y + 1] == EAU)
                return 1;
            else if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y + 1] == VERRE &&
                    verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y + 1) == VERRE_HAUT)
                return 1;
            break;

        case OUEST:
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == BORD &&
                bord(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == BORD_GAUCHE)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == VERRE &&
                verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == VERRE_GAUCHE)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x - 1][population[OUVRIERES][I].y] == COTON ||
                carte[PHYSIQUE][population[OUVRIERES][I].x - 1][population[OUVRIERES][I].y] == EAU)
                return 1;
            else if(carte[PHYSIQUE][population[OUVRIERES][I].x - 1][population[OUVRIERES][I].y] == VERRE &&
                    verre(carte, population[OUVRIERES][I].x - 1, population[OUVRIERES][I].y) == VERRE_DROITE)
                return 1;
            break;

        case NORD:
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == BORD &&
                bord(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == BORD_HAUT)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y] == VERRE &&
                verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y) == VERRE_HAUT)
                return 1;
            if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y - 1] == COTON ||
                carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y - 1] == EAU)
                return 1;
            else if(carte[PHYSIQUE][population[OUVRIERES][I].x][population[OUVRIERES][I].y - 1] == VERRE &&
                    verre(carte, population[OUVRIERES][I].x, population[OUVRIERES][I].y - 1) == VERRE_BAS)
                return 1;
            break;
    }

    return 0;
}


void deplacementReine(infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE])
{
    if(reine->vie > 0 && reine->delaiAnimation <= 0)
    {
        reine->directionPrecedente = reine->direction;

        do
        {
            reine->direction += ((rand() % 3) - 1);

            if(reine->direction < 0)
            {
                reine->direction += 4;
            }
            else if(reine->direction > 3)
            {
                reine->direction -= 4;
            }
        } while(gestionCollisionReine(reine, carte, reine->direction));

        switch(reine->direction)
        {
            case EST:
                reine->x++;
                break;
            case NORD:
                reine->y--;
                break;
            case OUEST:
                reine->x--;
                break;
            case SUD:
                reine->y++;
                break;
        }

        reine->delaiAnimation = 5;
    }

    else if(reine->delaiAnimation > 0)
        reine->delaiAnimation--;
}


int gestionCollisionReine(infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int direction)
{
    switch(direction)
    {
        case EST:
            if((carte[PHYSIQUE][reine->x + 1][reine->y] == BORD &&
               bord(carte, reine->x + 1, reine->y) == BORD_DROITE) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y + 1] == BORD &&
               bord(carte, reine->x + 1, reine->y + 1) == BORD_DROITE))
               return 1;
            if((carte[PHYSIQUE][reine->x + 1][reine->y] == VERRE &&
               verre(carte, reine->x + 1, reine->y) == VERRE_DROITE) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y + 1] == VERRE &&
               verre(carte, reine->x + 1, reine->y + 1) == VERRE_DROITE))
               return 1;
            if(carte[PHYSIQUE][reine->x + 2][reine->y] == COTON ||
               carte[PHYSIQUE][reine->x + 2][reine->y] == EAU ||
               carte[PHYSIQUE][reine->x + 2][reine->y + 1] == COTON ||
               carte[PHYSIQUE][reine->x + 2][reine->y + 1] == EAU)
                return 1;
            else if((carte[PHYSIQUE][reine->x + 2][reine->y] == VERRE &&
                    verre(carte, reine->x + 2, reine->y) == VERRE_GAUCHE) ||
                    (carte[PHYSIQUE][reine->x + 2][reine->y + 1] == VERRE &&
                    verre(carte, reine->x + 2, reine->y + 1) == VERRE_GAUCHE))
                return 1;
            break;

        case SUD:
            if((carte[PHYSIQUE][reine->x][reine->y + 1] == BORD &&
               bord(carte, reine->x, reine->y + 1) == BORD_BAS) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y + 1] == BORD &&
               bord(carte, reine->x + 1, reine->y + 1) == BORD_BAS))
               return 1;
            if((carte[PHYSIQUE][reine->x][reine->y + 1] == VERRE &&
               verre(carte, reine->x, reine->y + 1) == VERRE_BAS) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y + 1] == VERRE &&
               verre(carte, reine->x + 1, reine->y + 1) == VERRE_BAS))
               return 1;
            if(carte[PHYSIQUE][reine->x][reine->y + 2] == COTON ||
               carte[PHYSIQUE][reine->x][reine->y + 2] == EAU ||
               carte[PHYSIQUE][reine->x + 1][reine->y + 2] == COTON ||
               carte[PHYSIQUE][reine->x + 1][reine->y + 2] == EAU)
                return 1;
            else if((carte[PHYSIQUE][reine->x][reine->y + 2] == VERRE &&
                    verre(carte, reine->x, reine->y + 2) == VERRE_HAUT) ||
                    (carte[PHYSIQUE][reine->x + 1][reine->y + 2] == VERRE &&
                    verre(carte, reine->x + 1, reine->y + 2) == VERRE_HAUT))
                return 1;
            break;

        case OUEST:
            if((carte[PHYSIQUE][reine->x][reine->y] == BORD &&
               bord(carte, reine->x, reine->y) == BORD_GAUCHE) ||
               (carte[PHYSIQUE][reine->x][reine->y + 1] == BORD &&
               bord(carte, reine->x, reine->y + 1) == BORD_GAUCHE))
               return 1;
            if((carte[PHYSIQUE][reine->x][reine->y] == VERRE &&
               verre(carte, reine->x, reine->y) == VERRE_GAUCHE) ||
               (carte[PHYSIQUE][reine->x][reine->y + 1] == VERRE &&
               verre(carte, reine->x, reine->y + 1) == VERRE_GAUCHE))
               return 1;
            if(carte[PHYSIQUE][reine->x - 1][reine->y] == COTON ||
               carte[PHYSIQUE][reine->x - 1][reine->y] == EAU ||
               carte[PHYSIQUE][reine->x - 1][reine->y + 1] == COTON ||
               carte[PHYSIQUE][reine->x - 1][reine->y + 1] == EAU)
                return 1;
            else if((carte[PHYSIQUE][reine->x - 1][reine->y] == VERRE &&
                    verre(carte, reine->x - 1, reine->y) == VERRE_DROITE) ||
                    (carte[PHYSIQUE][reine->x - 1][reine->y + 1] == VERRE &&
                    verre(carte, reine->x - 1, reine->y + 1) == VERRE_DROITE))
                return 1;
            break;

        case NORD:
            if((carte[PHYSIQUE][reine->x][reine->y] == BORD &&
               bord(carte, reine->x, reine->y) == BORD_HAUT) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y] == BORD &&
               bord(carte, reine->x + 1, reine->y) == BORD_HAUT))
               return 1;
            if((carte[PHYSIQUE][reine->x][reine->y] == VERRE &&
               verre(carte, reine->x, reine->y) == VERRE_HAUT) ||
               (carte[PHYSIQUE][reine->x + 1][reine->y] == VERRE &&
               verre(carte, reine->x + 1, reine->y) == VERRE_HAUT))
               return 1;
            if(carte[PHYSIQUE][reine->x][reine->y - 1] == COTON ||
               carte[PHYSIQUE][reine->x][reine->y - 1] == EAU ||
               carte[PHYSIQUE][reine->x + 1][reine->y - 1] == COTON ||
               carte[PHYSIQUE][reine->x + 1][reine->y - 1] == EAU)
                return 1;
            else if((carte[PHYSIQUE][reine->x][reine->y - 1] == VERRE &&
                    verre(carte, reine->x, reine->y - 1) == VERRE_BAS) ||
                    (carte[PHYSIQUE][reine->x + 1][reine->y - 1] == VERRE &&
                    verre(carte, reine->x + 1, reine->y - 1) == VERRE_BAS))
                return 1;
            break;
    }

    return 0;
}


char* nomMois(int mois, char nomDuMois[])
{
    switch(mois)
    {
        case 1:
            sprintf(nomDuMois, "Jan");
            break;
        case 2:
            sprintf(nomDuMois, "Feb");
            break;
        case 3:
            sprintf(nomDuMois, "Mar");
            break;
        case 4:
            sprintf(nomDuMois, "Apr");
            break;
        case 5:
            sprintf(nomDuMois, "May");
            break;
        case 6:
            sprintf(nomDuMois, "Jun");
            break;
        case 7:
            sprintf(nomDuMois, "Jul");
            break;
        case 8:
            sprintf(nomDuMois, "Aug");
            break;
        case 9:
            sprintf(nomDuMois, "Sep");
            break;
        case 10:
            sprintf(nomDuMois, "Oct");
            break;
        case 11:
            sprintf(nomDuMois, "Nov");
            break;
        case 12:
            sprintf(nomDuMois, "Dec");
            break;
    }

    return nomDuMois;
}


void tutorial(SDL_Surface* ecran)
{

}


void charger(SDL_Surface* ecran)
{

}
