#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constantes.h"
#include "jeu.h"
#include "fichiers.h"
#include "parametres.h"
#include "graphisme.h"
#include "carte.h"


int main(int argc, char* argv[])
{
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect pos;
    SDL_Event event;

    int continuer = 1;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Impossible de charger la SDL : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetIcon(IMG_Load("sprites/icone_fourmi.png"), NULL);

    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le module video : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Simulateur d'elevage de fourmis", NULL);

    menu = IMG_Load("sprites/menu.png");


    while(continuer)
    {
        SDL_WaitEvent(&event);

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
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:

                // Si on clique sur la porte, on quitte le simulateur
                if(event.button.x >= 584 && event.button.x <= 719 && event.button.y >= 509 && event.button.y <= 690)
                {
                    continuer = 0;
                }

                // Si on clique sur "Play", on appelle la fonction jeu
                else if(event.button.x >= 223 && event.button.x <= 627 && event.button.y >= 260 && event.button.y <= 450)
                {
                    jouer(ecran);
                }

                // Si on clique sur "Load", on appelle la fonction charger
                else if(event.button.x >= 655 && event.button.x <= 1051 && event.button.y >= 60 && event.button.y <= 450)
                {
                    charger(ecran);
                }

                // Si on clique sur "Tutorial", on appelle la fonction tutorial
                else if(event.button.x >= 51 && event.button.x <= 445 && event.button.y >= 476 && event.button.y <= 664)
                {
                    tutorial(ecran);
                }

                // Si on clique sur "Options", on appelle la fonction options
                else if(event.button.x >= 833 && event.button.x <= 1236 && event.button.y >= 476 && event.button.y <= 664)
                {
                    options(ecran);
                }

                break;
            default:
                break;
        }


        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        pos.x = 0;
        pos.y = 0;
        SDL_BlitSurface(menu, NULL, ecran, &pos);

        SDL_Flip(ecran);
    }


    SDL_FreeSurface(menu);

    SDL_Quit();

    return EXIT_SUCCESS;
}
