#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED


typedef struct coordonnees coordonnees;
struct coordonnees
{
    int x;
    int y;
};

typedef struct infosCouvain infosCouvain;
struct infosCouvain
{
    int x;
    int y;

    int delai;
};

typedef struct infosPopulation infosPopulation;
struct infosPopulation
{
    int x;
    int y;

    int vie;

    int directionPrecedente;
    int direction;
    int delaiAnimation;

    int comportement;
};

void jouer(SDL_Surface* ecran);
void gestionPop(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION], int carteNbrPopulation[TAILLE_TABLEAU_POPULATION][LARGEUR_CARTE][HAUTEUR_CARTE], infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int temperature);
void gestionPontes(infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int carteNbrPopulation[TAILLE_TABLEAU_POPULATION][LARGEUR_CARTE][HAUTEUR_CARTE], long nbrPopulation[], int nbrReines, int temperature);
int meteo(int mois);
void reinitialisationSurfacesInutiles(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], infosCouvain couvain[TAILLE_TABLEAU_COUVAIN][MAX_POPULATION]);
void deplacementOuvrieres(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE]);
int gestionCollisionOuvriere(infosPopulation population[TAILLE_TABLEAU_POPULATION][MAX_POPULATION], int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int I);
void deplacementReine(infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE]);
int gestionCollisionReine(infosPopulation *reine, int carte[TAILLE_TABLEAU_CARTE][LARGEUR_CARTE][HAUTEUR_CARTE], int direction);
char* nomMois(int mois, char nomMois[]);
void tutorial(SDL_Surface* ecran);
void charger(SDL_Surface* ecran);


#endif // JEU_H_INCLUDED
