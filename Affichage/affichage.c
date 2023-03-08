#include "../src/include/SDL2/SDL.h"
#include "../src/include/SDL2/SDL_image.h"
#include "../src/include/SDL2/SDL_ttf.h"
#include "../src/include/SDL2/SDL_mixer.h"
#include "affichage.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------
#define LARGEUR_TILE 50
#define HAUTEUR_TILE 50
#define MUR 1
#define CHEMIN 0
#define ROCHER 2
#define BOMBE 3
#define BOMBE_EXPLOSION 4
#define WIN 5
#define LOOSE 6
#define POW 7
SDL_Rect VecteurPositionPersonnage;
SDL_Rect VecteurPositionPersonnage2;

/**
 * @struct BombeThread
 * @details structure du thread Bombe
 */
typedef struct BombeThread
{

    int map[13][13];
    int compteurBombe;
    SDL_Renderer *rendu;

} BT;
BT Liste_Args;
int ETAT = 0;
int Carte[13][13];
SDL_Renderer *renderer = NULL;

SDL_Window *fenetre = NULL;
int compteurBombe = 0;
int JeuxFini = 0;
int choixX = 0;

int choixY = 0;
int PowerUpP1POW = 0;
int PowerUpP2POW = 0;
int automatisation = 0;
int COMPTEUR_BOMBE = 0;
SDL_TimerID timerID;

//-----------------------------------------------------------------------------------
SDL_Surface *AffichagePersonnage2;
SDL_Texture *Personnage2 = NULL;
SDL_Surface *AffichagePersonnage;
SDL_Texture *Personnage1 = NULL;
SDL_Rect VecteurPosition;
SDL_Surface *AffichageTiles;
SDL_Surface *AffichageMurs;
SDL_Surface *AffichageChemin;
SDL_Surface *AffichageBomb;
SDL_Surface *AffichageBombExplosion;
SDL_Surface *AffichagePowerUp;

SDL_Texture *texture = NULL;
SDL_Texture *texture2 = NULL;
SDL_Texture *texture3 = NULL;
SDL_Texture *texture4 = NULL;
SDL_Texture *texture5 = NULL;

SDL_Surface *AffichageWin;
SDL_Texture *Win = NULL;

SDL_Surface *AffichageLoose;
SDL_Texture *Loose = NULL;
SDL_Texture *PowerUp = NULL;
//--------------------------------------------------------------------------------------------------------------
int menu()
{

    choixX = rand() % 11 + 1;
    choixY = rand() % 11 + 1;
    printf("Un PowerUp est apparu en [%d][%d] \n", choixX, choixY);
    Carte[choixX][choixY] = POW;

    if (TTF_Init() != 0)
    {
        SDL_Log("Erreur: Initialisation TTF_Init > %s\n ", TTF_GetError());
        return EXIT_FAILURE;
    };
    TTF_Font *TestNouvellePartie = TTF_OpenFont("police/ChinaOne.ttf", 30);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Erreur: Initialisation SDL_INIT_AUDIO > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    };

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        SDL_Log("Erreur: Initialisation audio mix_init > %s\n ", Mix_GetError());
        return EXIT_FAILURE;
    }

    Mix_Music *MusiqueEntree;
    MusiqueEntree = Mix_LoadMUS("musique/KungFuPanda1.mp3");

    if (TestNouvellePartie == NULL)
    {
        SDL_Log("Erreur: Initialisation TestNouvellePartie > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur: Initialisation SDL > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    };

    fenetre = SDL_CreateWindow("Projet Bomberman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 13 * HAUTEUR_TILE, 13 * LARGEUR_TILE, 0);
    if (fenetre == NULL)
    {
        SDL_Log("Erreur: Création fenetre > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Erreur: Création rendu > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderPresent(renderer);

    SDL_Surface *image = NULL;

    SDL_Color rouge = {255, 0, 0};

    image = IMG_Load("img/menu.png");

    SDL_Surface *EcritureNouvellePartie = TTF_RenderText_Solid(TestNouvellePartie, "Appuie sur 1 pour jouez une partie", rouge);

    if (image == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création Image > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    if (EcritureNouvellePartie == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création EcritureNouvellePartie > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Texture *TexturePourEcriture = SDL_CreateTextureFromSurface(renderer, EcritureNouvellePartie);

    SDL_FreeSurface(EcritureNouvellePartie);

    SDL_FreeSurface(image);
    if (texture == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création Texture > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    if (TexturePourEcriture == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création TexturePourEcriture > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Rect rectangle;
    SDL_Rect PositionEcriture;

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création Rectangle+Texture > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    if (SDL_QueryTexture(TexturePourEcriture, NULL, NULL, &PositionEcriture.w, &PositionEcriture.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: Création Rectangle+Texture > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }

    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 13 * HAUTEUR_TILE;
    rectangle.h = 13 * HAUTEUR_TILE;
    PositionEcriture.x = (800 - PositionEcriture.w) / 2;
    PositionEcriture.y = (1050 - PositionEcriture.h) / 2;

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: RenderCopy1 > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_RenderCopy(renderer, TexturePourEcriture, NULL, &PositionEcriture) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        SDL_Log("Erreur: RenderCopy2 > %s\n ", SDL_GetError());
        return EXIT_FAILURE;
    }
    AffichagePersonnage2=IMG_Load("img/Maitre_Singe.png");
    AffichagePersonnage=IMG_Load("img/Po-PD.png");
    AffichageTiles=IMG_Load("img/Bamboo-Bloc.png");
    AffichageMurs=IMG_Load("img/Rock-Bloc.png");
    AffichageChemin=IMG_Load("img/Chemin.png");
    AffichageBomb=IMG_Load("img/bomb.png");
    AffichageBombExplosion=IMG_Load("img/explosion_bombe.png");
    AffichageLoose=IMG_Load("img/gameover.jpg");
     AffichageWin=IMG_Load("img/youwin.png");
     AffichagePowerUp=IMG_Load("img/pow.png");
    if (AffichagePowerUp == NULL)
    {
        printf("Erruer Image PowerUp \n");
    }
    printf("Loading Image \n");

    texture = SDL_CreateTextureFromSurface(renderer, AffichageTiles);
    texture2 = SDL_CreateTextureFromSurface(renderer, AffichageMurs);
    texture3 = SDL_CreateTextureFromSurface(renderer, AffichageChemin);
    texture4 = SDL_CreateTextureFromSurface(renderer, AffichageBomb);
    texture5 = SDL_CreateTextureFromSurface(renderer, AffichageBombExplosion);

    Win = SDL_CreateTextureFromSurface(renderer, AffichageWin);
    Loose = SDL_CreateTextureFromSurface(renderer, AffichageLoose);
    Personnage2 = SDL_CreateTextureFromSurface(renderer, AffichagePersonnage2);
    PowerUp = SDL_CreateTextureFromSurface(renderer, AffichagePowerUp);
    if (PowerUp == NULL)
    {
        printf("Erreur Power up \n");
    }

    Personnage1 = SDL_CreateTextureFromSurface(renderer, AffichagePersonnage);

    Liste_Args.rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE);
    Liste_Args.compteurBombe = 0;
    VecteurPositionPersonnage.y = 50;
    VecteurPositionPersonnage.x = 50;
    VecteurPositionPersonnage.w = LARGEUR_TILE;
    VecteurPositionPersonnage.h = HAUTEUR_TILE;
    VecteurPositionPersonnage2.y = 550;
    VecteurPositionPersonnage2.x = 550;
    VecteurPositionPersonnage2.w = LARGEUR_TILE;
    VecteurPositionPersonnage2.h = HAUTEUR_TILE;

    SDL_bool programmeEnCours = SDL_TRUE;
    Mix_PlayMusic(MusiqueEntree, -1);
    while (programmeEnCours)
    {

        SDL_RenderPresent(renderer);
        SDL_Event evenement;
        while (SDL_PollEvent(&evenement))
        {
            switch (evenement.type)
            {
            case SDL_QUIT:
                programmeEnCours = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (evenement.key.keysym.sym)
                {
                case SDLK_SPACE:
                    programmeEnCours = SDL_FALSE;
                    Mix_CloseAudio();
                    break;
                case SDLK_a:
                    printf("Automatisation du 2eme perso \n");
                    automatisation++;
                    if (automatisation % 2 == 0)
                    {
                        pthread_t DeplacementAuto;
                        pthread_create(&DeplacementAuto, NULL, Moove_Perso, NULL);
                    }

                    break;
                case SDLK_1:

                    CreationMap(renderer, Carte);
                    InitialisationJoeur(renderer, Carte);
                    InitialisationJoeur2(renderer, Carte);
                    Carte[choixX][choixY] = POW;
                    ETAT = 1;

                    break;
                case SDLK_z:

                    printf("Valeur qui bug (Dans le deplacement avant le refresh) : %d \n", Carte[10][11]);
                    DeplacementPersonnage(renderer, 122, Carte);

                    break;
                case SDLK_q:

                    DeplacementPersonnage(renderer, 113, Carte);

                    break;
                case SDLK_s:

                    DeplacementPersonnage(renderer, 115, Carte);

                    break;
                case SDLK_d:

                    DeplacementPersonnage(renderer, SDLK_d, Carte);

                    break;
                case SDLK_e:
                    printf("Depot de bombe");

                    if (Liste_Args.compteurBombe == 0)
                    {
                        creation_bombe(COMPTEUR_BOMBE, Carte, renderer);
                    }
                    else
                    {
                        printf("Impossible Bombe explosion \n");
                    }

                    break;
                case SDLK_t:

                    SauvegardeGame(Carte);
                    break;

                case SDLK_r:
                    printf("Chargement en Cours \n");

                    ChargementGame(Carte);

                    break;
                case SDLK_o:
                    printf("Deplacement perso 2 \n");
                    DeplacementPersonnage2(renderer, SDLK_o, Carte);

                    break;
                case SDLK_m:
                    printf("Deplacement perso 2 \n");
                    DeplacementPersonnage2(renderer, SDLK_m, Carte);

                    break;
                case SDLK_l:
                    printf("Deplacement perso 2 \n");
                    DeplacementPersonnage2(renderer, SDLK_l, Carte);

                    break;
                case SDLK_k:
                    printf("Deplacement perso 2 \n");
                    DeplacementPersonnage2(renderer, SDLK_k, Carte);

                    break;
                case SDLK_p:
                    printf("Depot de bombe");
                    Liste_Args.rendu = renderer;
                    if (compteurBombe == 0)
                    {
                        creation_bombe2(COMPTEUR_BOMBE, Carte, renderer);
                    }
                    else
                    {
                        printf("Impossible Bombe explosion \n");
                    }

                    break;
                default:

                    break;
                }

            default:

                break;
            }
        }
        if (ETAT == 1)
        {
            frame_rate(24);
        }
    }

    printf("Fin de programme \n");
    SDL_RemoveTimer(timerID);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(TestNouvellePartie);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

void *Moove_Perso()
{
    int choix_moove = 0;
    int parterne[] = {0, 2, 4, 1, 0, 10, 2, 2, 4, 1, 1};
    if (automatisation == 2)
    {

        for (int i = 0; i < 11; i++)
        {

            if (parterne[i] == 0)
            {
                Sleep(750);
                VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y - HAUTEUR_TILE;
                printf("Paterne en cours \n");
            }
            else if (parterne[i] == 1)
            {
                Sleep(750);
                VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x + LARGEUR_TILE;
                printf("Paterne en cours \n");
            }
            else if (parterne[i] == 2)
            {
                Sleep(750);
                VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x - LARGEUR_TILE;
                printf("Paterne en cours \n");
            }
            else if (parterne[i] == 3)
            {
                Sleep(750);
                VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y + HAUTEUR_TILE;
                printf("Paterne en cours \n");
            }
            else if (parterne[i] == 10)
            {
                Sleep(750);
            }
            else
            {
                Sleep(250);
                creation_bombe2(COMPTEUR_BOMBE, Carte, renderer);

                printf("Paterne en cours \n");
            }
        }
    }

    int res;
    Sleep(750);
    printf("VecteurPositionPersonnage2: [%d][%d]", VecteurPositionPersonnage2.x / 50, VecteurPositionPersonnage2.y / 50);
    while (automatisation % 2 == 0)
    {

        choix_moove = rand() % 5;
        printf("Valeur Automatisation %d \n", automatisation);
        printf("CHoix_Moove : %d \n", choix_moove);

        switch (choix_moove)
        {
        case 0:
            Sleep(500);

            res = blocage2(Carte, SDLK_o);
            if (res == 0)
            {

                VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y - HAUTEUR_TILE;
            }
            break;
        case 1:
            Sleep(500);

            res = blocage2(Carte, SDLK_m);
            if (res == 0)
            {

                VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x + LARGEUR_TILE;
            }
            break;
        case 2:
            Sleep(500);

            res = blocage2(Carte, SDLK_k);
            if (res == 0)
            {

                VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x - LARGEUR_TILE;
            }
            break;
        case 3:
            Sleep(500);

            res = blocage2(Carte, SDLK_l);
            if (res == 0)
            {

                VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y + HAUTEUR_TILE;
            }
            break;
        case 4:
            Sleep(500);

            if (COMPTEUR_BOMBE == 0)
            {
                creation_bombe2(COMPTEUR_BOMBE, Carte, renderer);
            }
            else
            {
                printf("Impossible \n");
            }
        }
    }
    return NULL;
}

void frame_rate(int fps)
{
    static Uint32 prev_ticks = 0;
    Uint32 ticks = SDL_GetTicks();
    Uint32 min_ticks = 1000 / fps;
    if (ticks - prev_ticks < min_ticks)
    {
        SDL_Delay(min_ticks - (ticks - prev_ticks));
        RefreshEcran(Carte, renderer);
    }
    prev_ticks = SDL_GetTicks();
}

void LimiteActionBot(int actionPerSeconde)
{
    static Uint32 prev_ticks = 0;
    Uint32 ticks = SDL_GetTicks();
    Uint32 min_ticks = 1000 / actionPerSeconde;
    if (ticks - prev_ticks < min_ticks)
    {
        SDL_Delay(min_ticks - (ticks - prev_ticks));

        Moove_Perso();
    }
    prev_ticks = SDL_GetTicks();
}

void creation_bombe(int compB, int map[13][13], SDL_Renderer *rendu)
{
    pthread_t AffichageBombe;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            Liste_Args.map[i][j] = map[i][j];
        }
    }
    pthread_create(&AffichageBombe, NULL, gestion_bombe, &Liste_Args);

    printf("Fin du pthread_create \n");
}
Uint32 gestion_bombe_test()
{
    printf("Add Timer \n");
    int coX = VecteurPositionPersonnage.x / 50;
    int coY = VecteurPositionPersonnage.y / 50;
    Carte[coX][coY] = BOMBE;

    return 1;
}
void creation_bombe2(int compB, int map[13][13], SDL_Renderer *rendu)
{
    pthread_t AffichageBombe;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            Liste_Args.map[i][j] = map[i][j];
        }
    }
    pthread_create(&AffichageBombe, NULL, gestion_bombe2, &Liste_Args);

    printf("Fin du pthread_create \n");
}

/**
 * @brief gestion_bombe2
 * @param arg C'est un pointeur vers un type void (tt les types), c'est en général, une structure avec tt les arguments à l'interieur
 * @details Cette fonction permet de gerer les bombes dans un thread autre que le main (sans blocker donc le thread principal)
 * @ingroup affichage
 */
void *gestion_bombe2(void *arg)
{

    printf("La bombe va explose");

    int COX_Perso = VecteurPositionPersonnage2.x / 50;
    int COY_Perso = VecteurPositionPersonnage2.y / 50;

    if (Carte[COX_Perso][COY_Perso] == CHEMIN)
    {

        printf("Valeur Bombe Liste_Args %d \n", compteurBombe);
        if (compteurBombe != 0)
        {
            printf("Bombe deja actif \n");
        }
        else
        {

            int i = VecteurPositionPersonnage2.x / 50;
            int j = VecteurPositionPersonnage2.y / 50;
            Carte[i][j] = BOMBE;

            compteurBombe++;
            SDL_Delay(2000);
            ExplosionInGame(i, j, 2);
            if (Carte[COX_Perso - 1][COY_Perso] == BOMBE_EXPLOSION)
            {

                Carte[COX_Perso - 1][COY_Perso] = CHEMIN;
            }
            if (Carte[COX_Perso][COY_Perso - 1] == BOMBE_EXPLOSION)
            {

                Carte[COX_Perso][COY_Perso - 1] = CHEMIN;
            }
            if (Carte[COX_Perso][COY_Perso + 1] == BOMBE_EXPLOSION)
            {
                Carte[COX_Perso][COY_Perso + 1] = CHEMIN;
            }
            if (Carte[COX_Perso + 1][COY_Perso] == BOMBE_EXPLOSION)
            {
                Carte[COX_Perso + 1][COY_Perso] = CHEMIN;
            }
            if (PowerUpP2POW == 1)
            {
                if (Carte[COX_Perso + 2][COY_Perso] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso + 2][COY_Perso] = CHEMIN;
                }
                if (Carte[COX_Perso - 2][COY_Perso] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso - 2][COY_Perso] = CHEMIN;
                }
                if (Carte[COX_Perso][COY_Perso + 2] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso][COY_Perso + 2] = CHEMIN;
                }
                if (Carte[COX_Perso][COY_Perso - 2] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso][COY_Perso - 2] = CHEMIN;
                }
            }
            compteurBombe--;
            Carte[i][j] = CHEMIN;
        }
    }
    else
    {
        printf("Impossible de pose la bombe \n");

        return NULL;
    }

    printf("La bombe a explose \n");

    ConditionVictoire(Carte, 2, COX_Perso, COY_Perso);

    return 0;
}

/**
 * @brief gestion_bombe
 * @param arg C'est un pointeur vers un type void (tt les types), c'est en général, une structure avec tt les arguments à l'interieur
 * @details Cette fonction permet de gerer les bombes dans un thread autre que le main (sans blocker donc le thread principal)
 * @ingroup affichage
 */
void *gestion_bombe(void *arg)
{

    printf("La bombe va explose");

    int COX_Perso = VecteurPositionPersonnage.x / 50;
    int COY_Perso = VecteurPositionPersonnage.y / 50;

    if (Liste_Args.map[COX_Perso][COY_Perso] == CHEMIN)
    {

        printf("Valeur Bombe Liste_Args %d \n", Liste_Args.compteurBombe);
        if (Liste_Args.compteurBombe != 0)
        {
            printf("Bombe deja actif \n");
        }
        else
        {

            int i = VecteurPositionPersonnage.x / 50;
            int j = VecteurPositionPersonnage.y / 50;
            Carte[i][j] = BOMBE;

            Liste_Args.compteurBombe++;
            SDL_Delay(2000);

            ExplosionInGame(i, j, 1);

            if (Carte[COX_Perso - 1][COY_Perso] == BOMBE_EXPLOSION)
            {

                Carte[COX_Perso - 1][COY_Perso] = CHEMIN;
            }
            if (Carte[COX_Perso][COY_Perso - 1] == BOMBE_EXPLOSION)
            {

                Carte[COX_Perso][COY_Perso - 1] = CHEMIN;
            }
            if (Carte[COX_Perso][COY_Perso + 1] == BOMBE_EXPLOSION)
            {
                Carte[COX_Perso][COY_Perso + 1] = CHEMIN;
            }
            if (Carte[COX_Perso + 1][COY_Perso] == BOMBE_EXPLOSION)
            {
                Carte[COX_Perso + 1][COY_Perso] = CHEMIN;
            }
            if (PowerUpP1POW == 1)
            {
                if (Carte[COX_Perso + 2][COY_Perso] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso + 2][COY_Perso] = CHEMIN;
                }
                if (Carte[COX_Perso - 2][COY_Perso] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso - 2][COY_Perso] = CHEMIN;
                }
                if (Carte[COX_Perso][COY_Perso + 2] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso][COY_Perso + 2] = CHEMIN;
                }
                if (Carte[COX_Perso][COY_Perso - 2] == BOMBE_EXPLOSION)
                {
                    Carte[COX_Perso][COY_Perso - 2] = CHEMIN;
                }
            }
            Liste_Args.compteurBombe--;
            Carte[i][j] = CHEMIN;
        }
    }
    else
    {
        printf("Impossible de pose la bombe \n");

        return NULL;
    }

    printf("La bombe a explose \n");

    ConditionVictoire(Carte, 1, COX_Perso, COY_Perso);

    return 0;
}

/**
 * @brief SauvegardeGame
 * @param map La matrice qui représente la carte de jeu
 * @details Cette fonction permet de sauvegarde une partie dans une sauvegarde en TXT, elle sauvegardera le statut de chaque case et la position des joueurs
 * @ingroup affichage
 */
void SauvegardeGame(int carte[13][13])
{
    printf("Sauvegarde en cours \n");
    FILE *fp;
    char temp[10];

    fp = fopen("sauvegarde.txt", "w");
    if (fp == NULL)
    {

        printf("erreur ouverture sauvegarde");
    }
    else
    {
        printf("fichier correctement ouvert/ecrit \n");

        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                sprintf(temp, "%d", carte[i][j]);
                fputs(temp, fp);
            }
        }
    }

    fclose(fp);
}

/**
 * @brief ChargementGame
 * @param map La matrice qui représente la carte de jeu
 * @details Cette fonction permet de charger une partie depuis une sauvegarde en TXT, elle modifie ensuite la carte de jeu et affiche la modification
 * @ingroup affichage
 */
void ChargementGame(int carte[13][13])
{
    FILE *fp;

    fp = fopen("sauvegarde.txt", "r");
    printf("Valeur [%d]", VecteurPositionPersonnage.x);
    printf("Valeur [%d]", VecteurPositionPersonnage.y);
    char parcourt;

    int i = 0;
    int j = 0;
    if (fp == NULL)
    {

        printf("erreur ouverture sauvegarde");
    }
    else
    {

        while (parcourt != EOF)
        {
            parcourt = fgetc(fp);

            if (parcourt == '0')
            {
                carte[i][j] = CHEMIN;
            }
            else if (parcourt == '1')
            {
                carte[i][j] = MUR;
            }
            else if (parcourt == '2')
            {
                carte[i][j] = ROCHER;
            }
            else if (parcourt == '3')
            {
                carte[i][j] = BOMBE;
            }

            else
            {
                printf("Autre cas (espace) \n");
            }
            j++;
            if (j == 13)
            {
                j = 0;

                i++;
            }
        }
    }

    fclose(fp);
}

/**
 * @brief CreationMap
 * @param map La matrice qui représente la carte de jeu
 * @param rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction initialise la carte de jeu et donne le bon statut aux cases de la matrice
 * @ingroup affichage
 */
void CreationMap(SDL_Renderer *rendu, int map[13][13])
{
    SDL_RenderClear(rendu);
    PowerUpP1POW = 0;
    PowerUpP2POW = 0;

    VecteurPositionPersonnage.y = 50;
    VecteurPositionPersonnage.x = 50;
    VecteurPositionPersonnage.w = LARGEUR_TILE;
    VecteurPositionPersonnage.h = HAUTEUR_TILE;
    VecteurPositionPersonnage2.y = 550;
    VecteurPositionPersonnage2.x = 550;
    VecteurPositionPersonnage2.w = LARGEUR_TILE;
    VecteurPositionPersonnage2.h = HAUTEUR_TILE;
    SDL_Rect VecteurPosition;
    SDL_Surface *AffichageTiles = IMG_Load("img/Bamboo-Bloc.png");
    SDL_Surface *AffichageMurs = IMG_Load("img/Rock-Bloc.png");
    SDL_Surface *AffichageChemin = IMG_Load("img/Chemin.png");

    SDL_Texture *texture = NULL;
    SDL_Texture *texture2 = NULL;
    SDL_Texture *texture3 = NULL;

    texture = SDL_CreateTextureFromSurface(rendu, AffichageTiles);
    texture2 = SDL_CreateTextureFromSurface(rendu, AffichageMurs);
    texture3 = SDL_CreateTextureFromSurface(rendu, AffichageChemin);

    VecteurPosition.w = LARGEUR_TILE;
    VecteurPosition.h = HAUTEUR_TILE;
    printf("Creation Map");

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {

            if (j % 2 != 0 && i % 2 == 0 && j > 0 && i > 0 && j < 12 && i < 12)
            {

                Carte[i][j] = ROCHER;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }

            else if (i == 11 && j == 10)
            {
                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if ((i == 10 && j == 11))
            {
                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if ((i == 1 && j == 2))
            {
                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if ((i == 2 && j == 1))
            {
                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if (j % 2 == 0 && i % 2 != 0 && j > 0 && i > 0 && j < 12 && i < 12)
            {

                Carte[i][j] = ROCHER;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }
            else if (j % 2 == 0 && i % 2 == 0 && j > 0 && i > 0 && j < 12 && i < 12)
            {

                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if (j % 2 != 0 && i % 2 != 0 && j > 0 && i > 0 && j < 12 && i < 12)
            {

                Carte[i][j] = CHEMIN;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else
            {
                Carte[i][j] = MUR;
                VecteurPosition.x = i * LARGEUR_TILE;
                VecteurPosition.y = j * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition);
            }
        }
    }
    choixX = rand() % 11 + 1;
    choixY = rand() % 11 + 1;
    printf("Un PowerUp est apparu en [%d][%d] \n", choixX, choixY);
    Carte[choixX][choixY] = POW;

    SDL_RenderPresent(rendu);
}

//--------------------------------------------------------------------------------------------------------------

/**
 * @brief
 *  InitialisationJoeur
 * @param map La matrice qui représente la carte de jeu
 * @param rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction initialise la texture des joueurs sur la carte pour la première fois et affiche la modification
 * @ingroup affichage
 */
void InitialisationJoeur(SDL_Renderer *rendu, int map[13][13])
{

    SDL_Rect VecteurPosition;
    SDL_Surface *AffichagePersonnage = IMG_Load("img/Po-PD.png");
    SDL_Texture *Personnage1 = NULL;

    VecteurPosition.w = LARGEUR_TILE;
    VecteurPosition.h = HAUTEUR_TILE;
    Personnage1 = SDL_CreateTextureFromSurface(rendu, AffichagePersonnage);
    VecteurPosition.x = LARGEUR_TILE;
    VecteurPosition.y = HAUTEUR_TILE;

    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPosition);
}

/**
 * @brief InitialisationJoeur2
 * @param map La matrice qui représente la carte de jeu
 * @param rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction initialise la texture des joueurs sur la carte pour la première fois et affiche la modification
 * @ingroup affichage
 */
void InitialisationJoeur2(SDL_Renderer *rendu, int map[13][13])
{

    SDL_Surface *AffichagePersonnage = IMG_Load("img/Maitre_Singe.png");
    SDL_Texture *Personnage1 = NULL;

    Personnage1 = SDL_CreateTextureFromSurface(rendu, AffichagePersonnage);
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
}

//--------------------------------------------------------------------------------------------------------------
/**
 * @brief DéplacementPersonnage
 * @param map La matrice qui représente la carte de jeu
 * @param touche La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
 * @param rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction permet d'effectuer le déplacement du personnage en fonction de la direction
 * du déplacement (touche), elle appelera la fonction blocage pour savoir si le déplacement est possible
 * On affiche ensuite sur la fenetre de jeu , la nouvelle position du personnage après déplacement
 * @ingroup affichage
 */
void DeplacementPersonnage(SDL_Renderer *rendu, int touche, int Carte[13][13])
{

    SDL_Surface *AffichagePersonnage = IMG_Load("img/Po-PD.png");
    SDL_Texture *Personnage1 = NULL;
    Personnage1 = SDL_CreateTextureFromSurface(rendu, AffichagePersonnage);

    if (touche == 122)
    {
        int res = blocage(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage.y = VecteurPositionPersonnage.y - HAUTEUR_TILE;
            printf("Deplacement vers le haut\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
    else if (touche == 113)
    {
        int res = blocage(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage.x = VecteurPositionPersonnage.x - LARGEUR_TILE;
            printf("Deplacement vers le gauche\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }

    else if (touche == 115)
    {
        int res = blocage(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage.y = VecteurPositionPersonnage.y + HAUTEUR_TILE;
            printf("Deplacement vers le bas\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
    else
    {
        int res = blocage(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage.x = VecteurPositionPersonnage.x + LARGEUR_TILE;
            printf("Deplacement vers la droite\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
    int test = 0;
    test = VecteurPositionPersonnage.x / 50;
    int test2 = 0;
    test2 = VecteurPositionPersonnage.y / 50;
    printf("Perso en [%d][%d] \n", test, test2);
    if (test == choixX && test2 == choixY)
    {
        printf("augmentation de la taille de bombe \n");
        PowerUpP1POW = 1;
        Carte[choixX][choixY] = CHEMIN;
    }
    else
    {
        printf("Pas de PowerUp \n");
    }
}

/**
 * @brief DéplacementPersonnage2
 * @param map La matrice qui représente la carte de jeu
 * @param touche La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
 * @param rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction permet d'effectuer le déplacement du personnage2 en fonction de la direction
 * du déplacement (touche), elle appelera la fonction blocage2 pour savoir si le déplacement est possible
 * on affiche ensuite sur la fenetre de jeu , la nouvelle position du personnage après déplacement
 * @ingroup affichage
 */
void DeplacementPersonnage2(SDL_Renderer *rendu, int touche, int Carte[13][13])
{

    SDL_Surface *AffichagePersonnage = IMG_Load("img/Maitre_Singe.bmp");
    SDL_Texture *Personnage1 = NULL;
    Personnage1 = SDL_CreateTextureFromSurface(rendu, AffichagePersonnage);
   
    if (touche == SDLK_o)
    {
        int res = blocage2(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y - HAUTEUR_TILE;
            printf("Deplacement vers le haut\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }
    else if (touche == SDLK_k)
    {
        int res = blocage2(Carte, touche);
        printf("Test");
        if (res == 0)
        {

            printf("Test dans le res");

            VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x - LARGEUR_TILE;
            printf("Deplacement vers le gauche\n");
            printf("%d %d ", VecteurPositionPersonnage2.x, VecteurPositionPersonnage2.y);

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }

    else if (touche == SDLK_l)
    {
        int res = blocage2(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage2.y = VecteurPositionPersonnage2.y + HAUTEUR_TILE;
            printf("Deplacement vers le bas\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }
    else
    {
        int res = blocage2(Carte, touche);
        if (res == 0)
        {

            VecteurPositionPersonnage2.x = VecteurPositionPersonnage2.x + LARGEUR_TILE;
            printf("Deplacement vers la droite\n");

            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }
    int test = 0;
    test = VecteurPositionPersonnage2.x / 50;
    int test2 = 0;
    test2 = VecteurPositionPersonnage2.y / 50;
    printf("Perso en [%d][%d] \n", test, test2);
    if (test == choixX && test2 == choixY)
    {
        printf("augmentation de la taille de bombe \n");
        PowerUpP2POW = 1;
        Carte[choixX][choixY] = CHEMIN;
    }
    else
    {
        printf("Pas de PowerUp \n");
    }
}

//--------------------------------------------------------------------------------------------------------------
/**
 * @brief Blocage2
 * @param map La matrice qui représente la carte de jeu
 * @param touche La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
 * @details Cette fonction permet de tester si le déplacement du personnage2 est possible en fonction de la direction
 * du mouvement (touche) et également de la case visé par ce déplacement
 * Le déplacement est seulement possible si la case où l'on veut se déplacer est un chemin
 * @ingroup affichage
 */
int blocage2(int map[13][13], int touche)
{

    int V1 = 0;

    int CoX = VecteurPositionPersonnage2.x / 50;
    int CoY = VecteurPositionPersonnage2.y / 50;
    printf("%d %d ", CoX, CoY);

    if (touche == SDLK_o)
    {
        V1 = map[CoX][CoY - 1];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer haut\n");
            return -1;
        }
        return 0;
    }
    if (touche == SDLK_l)
    {
        V1 = map[CoX][CoY + 1];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer bas\n");
            return -1;
        }
        return 0;
    }
    if (touche == SDLK_k)
    {
        V1 = map[CoX - 1][CoY];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer gauche\n");
            return -1;
        }
        return 0;
    }
    if (touche == SDLK_m)
    {
        V1 = map[CoX + 1][CoY];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer droite\n");
            return -1;
        }
        return 0;
    }
    return -1;
}

/**
 * @brief Blocage
 * @param map La matrice qui représente la carte de jeu
 * @param touche La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
 * @details Cette fonction permet de tester si le déplacement du personnage est possible en fonction de la direction
 * du mouvement (touche) et également de la case visé par ce déplacement
 * Le déplacement est seulement possible si la case où l'on veut se déplacer est un chemin
 * @ingroup affichage
 */
int blocage(int map[13][13], int touche)
{

    int V1 = 0;

    int CoX = VecteurPositionPersonnage.x / 50;
    int CoY = VecteurPositionPersonnage.y / 50;

    if (touche == 122)
    {
        V1 = map[CoX][CoY - 1];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer haut\n");
            return -1;
        }
        return 0;
    }
    if (touche == 115)
    {
        V1 = map[CoX][CoY + 1];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer bas\n");
            return -1;
        }
        return 0;
    }
    if (touche == 113)
    {
        V1 = map[CoX - 1][CoY];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer gauche\n");
            return -1;
        }
        return 0;
    }
    if (touche == 100)
    {
        V1 = map[CoX + 1][CoY];
        if (V1 == MUR || V1 == ROCHER)
        {
            printf("bloquer droite\n");
            return -1;
        }
        return 0;
    }
    return -1;
}

/**
 * @brief
 *  RefreshEcran
 * @param map La matrice qui représente la carte de jeu
 * rendu C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
 * @details Cette fonction modifie l'affichage de la fenetre en fonction de la carte de jeu
 * On recrée toute la fenetre à chaque appel et on test toutes les cases de la matrice pour copier
 * dans le rendu la bonne texture
 * @ingroup affichage
 */
void RefreshEcran(int map[13][13], SDL_Renderer *rendu)
{

    if (SDL_RenderClear(rendu) == 0)
    {
    };

    if (AffichageTiles == NULL)
    {
        printf("Erreur chargement Tiles \n");
    }

    VecteurPosition.w = LARGEUR_TILE;
    VecteurPosition.h = HAUTEUR_TILE;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            VecteurPosition.x = i * LARGEUR_TILE;
            VecteurPosition.y = j * HAUTEUR_TILE;
            if (map[i][j] == CHEMIN)
            {
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if (map[i][j] == POW)
            {

                SDL_RenderCopy(rendu, PowerUp, NULL, &VecteurPosition);
            }
            else if (map[i][j] == MUR)
            {
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition);
            }
            else if (map[i][j] == ROCHER)
            {
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }
            else if (map[i][j] == BOMBE)
            {
                SDL_RenderCopy(rendu, texture4, NULL, &VecteurPosition);
            }
            else if (map[i][j] == BOMBE_EXPLOSION)
            {
                SDL_RenderCopy(rendu, texture5, NULL, &VecteurPosition);
            }

            else if (map[0][0] == WIN)
            {
                VecteurPosition.x = 0;
                VecteurPosition.y = 0;
                VecteurPosition.h = 13 * HAUTEUR_TILE;
                VecteurPosition.w = 13 * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, Win, NULL, &VecteurPosition);
            }
            else if (map[0][0] == LOOSE)
            {
                VecteurPosition.x = 0;
                VecteurPosition.y = 0;
                VecteurPosition.h = 13 * HAUTEUR_TILE;
                VecteurPosition.w = 13 * HAUTEUR_TILE;
                SDL_RenderCopy(rendu, Loose, NULL, &VecteurPosition);
            }
        }
    }

    SDL_RenderCopy(rendu, Personnage2, NULL, &VecteurPositionPersonnage2);
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);

    SDL_RenderPresent(rendu);
}

/**
 * @brief
 *  AfficheEndGame
 * @details C'est une fonction test pour afficher GameOver ou YouWin à la fin de la partie
 * @ingroup affichage
 */
void AfficheEndGame()
{
    SDL_RenderClear(renderer);
    SDL_Surface *AffichageWin = IMG_Load("img/youwin.png");
    SDL_Texture *Win = NULL;

    SDL_Surface *AffichageLoose = IMG_Load("img/gameover.jpg");
    SDL_Texture *Loose = NULL;

    Win = SDL_CreateTextureFromSurface(renderer, AffichageWin);
    Loose = SDL_CreateTextureFromSurface(renderer, AffichageLoose);

    SDL_Rect VecteurPosition;
    if (Carte[0][0] == WIN)
    {
        VecteurPosition.x = 0;
        VecteurPosition.y = 0;
        VecteurPosition.h = 25 * HAUTEUR_TILE;
        VecteurPosition.w = 25 * HAUTEUR_TILE;
        SDL_RenderCopy(renderer, Win, NULL, &VecteurPosition);
    }
    else if (Carte[0][0] == LOOSE)
    {
        VecteurPosition.x = 0;
        VecteurPosition.y = 0;
        VecteurPosition.h = 100 * HAUTEUR_TILE;
        VecteurPosition.w = 100 * HAUTEUR_TILE;
        SDL_RenderCopy(renderer, Loose, NULL, &VecteurPosition);
    }
    SDL_RenderPresent(renderer);
}

/**
 * @brief
 *  ExplosionInGame
 * @param coX c'est la coordonée en X depuis laquelle la fonction a été appelé
 * @param coY c'est la coordonée en X depuis laquelle la fonction a été appelé
 * @details En prenant en compte les coordonnées passées en paramètres, la fonction test les 4
 * possibilitées et modifie la carte en BOMBE_EXPLOSION , c'est à dire le cas où la bombe est en train d'exploser
 * On rafraichit ensuite la fenetre pour faire apparaitre la modification
 * @ingroup affichage
 */
void ExplosionInGame(int CoX, int CoY, int appelPerso)
{

    int V1 = Carte[CoX][CoY - 1];
    if (V1 == ROCHER || V1 == CHEMIN)
    {
        Carte[CoX][CoY - 1] = BOMBE_EXPLOSION;
    }
    V1 = Carte[CoX - 1][CoY];
    if (V1 == ROCHER || V1 == CHEMIN)
    {
        Carte[CoX - 1][CoY] = BOMBE_EXPLOSION;
    }
    V1 = Carte[CoX + 1][CoY];
    if (V1 == ROCHER || V1 == CHEMIN)
    {
        Carte[CoX + 1][CoY] = BOMBE_EXPLOSION;
    }
    V1 = Carte[CoX][CoY + 1];
    if (V1 == ROCHER || V1 == CHEMIN)
    {
        Carte[CoX][CoY + 1] = BOMBE_EXPLOSION;
    }
    if (PowerUpP1POW == 1 && appelPerso == 1)
    {
        V1 = Carte[CoX][CoY + 2];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX][CoY + 2] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX][CoY - 2];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX][CoY - 2] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX + 2][CoY];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX + 2][CoY] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX - 2][CoY];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX - 2][CoY] = BOMBE_EXPLOSION;
        }
    }
    if (PowerUpP2POW == 1 && appelPerso == 2)
    {
        V1 = Carte[CoX][CoY + 2];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX][CoY + 2] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX][CoY - 2];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX][CoY - 2] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX + 2][CoY];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX + 2][CoY] = BOMBE_EXPLOSION;
        }
        V1 = Carte[CoX - 2][CoY];
        if (V1 == ROCHER || V1 == CHEMIN)
        {
            Carte[CoX - 2][CoY] = BOMBE_EXPLOSION;
        }
    }

    printf("Dans l'explosion in game (rendu?) \n");

    printf("Appel en cours de RefreshEcran avec le renderer2 \n");

    SDL_Delay(500);
}

/**
 * @brief ConditionVictoire
 * @param Carte La matrice qui représente la carte de jeu
 * @param appelPerso permet d'identifier qui appel la fonction (le joueur 1 ou le joueur 2), utile pour savoir qui a gagner/perdu la partie
 * @param coX c'est la coordonée en X depuis laquelle la fonction a été appelé
 * @param coY c'est la coordonée en X depuis laquelle la fonction a été appelé
 * @details Identifie si un joueur est présent sur une case explosion lors de l'appel de la fonction apres les Sleeps
 * Modifie ensuite la carte de jeu en fonction du statut du JOUEUR 1 (WIN/LOOSE)
 * On Refresh alors l'écran pour faire apparaitre la modification
 * @ingroup affichage
 */
void ConditionVictoire(int Carte[13][13], int appelPerso, int coX, int coY)
{

    int CoX_Perso1 = VecteurPositionPersonnage.x / 50;
    int CoY_Perso1 = VecteurPositionPersonnage.y / 50;
    int CoX_Perso2 = VecteurPositionPersonnage2.x / 50;
    int CoY_Perso2 = VecteurPositionPersonnage2.y / 50;

    if ((CoX_Perso1 == coX && CoY_Perso1 == coY) || (CoX_Perso1 == coX - 1 && CoY_Perso1 == coY) || (CoX_Perso1 == coX + 1 && CoY_Perso1 == coY) || (CoX_Perso1 == coX && CoY_Perso1 == coY - 1) || (CoX_Perso1 == coX && CoY_Perso1 == coY + 1))
    {
        if (appelPerso == 1)
        {
            printf("Le joueur 1 est mort par suicide \n");
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 13; j++)
                {

                    Carte[i][j] = LOOSE;
                }
            }
            Sleep(250);

            printf("valeur qui bug : %d", Carte[10][11]);

            printf("Appel du refresh Ecran quand le perso1 se suicide \n");
        }

        else
        {
            printf("Le joueur 1 est mort par la bombe du joueur 2 \n");
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 13; j++)
                {

                    Carte[i][j] = LOOSE;
                }
            }
        }
    }
    else if (PowerUpP1POW == 1)
    {
        printf("Entre dans la condition \n");
        printf("Coo du joueur 2 [%d][%d] \n", CoX_Perso2, CoX_Perso2);
        printf("Cas 1 : [%d][%d] \n", coX - 2, coY);
        printf("Cas 2 : [%d][%d] \n", coX + 2, coY);
        printf("Cas 3 : [%d][%d] \n", coX, coY + 2);
        printf("Cas 4 : [%d][%d] \n", coX, coY - 2);
        if (((CoX_Perso1 == coX - 2 && CoY_Perso1 == coY) || (CoX_Perso1 == coX + 2 && CoY_Perso1 == coY) || (CoX_Perso1 == coX && CoY_Perso1 == coY - 2) || (CoX_Perso1 == coX && CoY_Perso1 == coY + 2)))
        {
            printf("Suicide avec l'augmentation \n'");
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 13; j++)
                {

                    Carte[i][j] = LOOSE;
                }
            }
        }
    }
    if ((CoX_Perso2 == coX && CoY_Perso2 == coY) || (CoX_Perso2 == coX - 1 && CoY_Perso2 == coY) || (CoX_Perso2 == coX + 1 && CoY_Perso2 == coY) || (CoX_Perso2 == coX && CoY_Perso2 == coY - 1) || (CoX_Perso2 == coX && CoY_Perso2 == coY + 1))
    {
        if (appelPerso == 2)
        {
            printf("Le joueur 2 est mort par suicide \n");

            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 13; j++)
                {

                    Carte[i][j] = WIN;
                }
            }

            printf("Appel du refreshEcran quand le perso1 se suicide \n");
        }
        else if (PowerUpP2POW == 1)
        {
            printf("Entre dans la condition \n");
            printf("Coo du joueur 2 [%d][%d] \n", CoX_Perso2, CoX_Perso2);
            printf("Cas 1 : [%d][%d] \n", coX - 2, coY);
            printf("Cas 2 : [%d][%d] \n", coX + 2, coY);
            printf("Cas 3 : [%d][%d] \n", coX, coY + 2);
            printf("Cas 4 : [%d][%d] \n", coX, coY - 2);
            if (((CoX_Perso2 == coX - 2 && CoY_Perso2 == coY) || (CoX_Perso2 == coX + 2 && CoY_Perso2 == coY) || (CoX_Perso2 == coX && CoY_Perso2 == coY - 2) || (CoX_Perso2 == coX && CoY_Perso2 == coY + 2)))
            {
                printf("Suicide avec l'augmentation \n'");
                for (int i = 0; i < 13; i++)
                {
                    for (int j = 0; j < 13; j++)
                    {

                        Carte[i][j] = LOOSE;
                    }
                }
            }
        }
        else
        {
            printf("Le joueur 2 est mort par la bombe du joueur 1 \n");
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 13; j++)
                {

                    Carte[i][j] = WIN;
                }
            }
        }
    }
}

// Bug à regler:

//- Sauvegarde également la position des joueurs dans la sauvegarde pour ne pas avoir de posisiton aléatoires après le chargement (OUBLIE DE MA PART)

//- Si les joueurs utilisent leurs bombes en simultané alors 1 des 2 bombes ne disparait pas (Encore un prblm de syncro , c'est vraiment la galère
// avec les threads)
//- Refresh Ecran après l'explosion de la bombe ne marche pas (crash après l'affichage) celui la j'ai pas compris , vraiment
//- 2 cases bizarres quand on win/loose (je pense que c'est un prblm de syncro avec la fin du threat , il faudrait jeter un oeil à la fin
// des fonctions getions_bombe) (DONE)

//- Placement des persos quand on rejoue (DONE)

// - Perso 2 disparait si suicide (Je pense que c'est un oublie de ma part , soit je ne charge pas les textures soir prblm sur le VecteurPoisition) (DONE)

// Features:
//-PowerUp pour pimenter la partie (DONE)
//-Implémentation d'une "IA"pour jouer tt seule

// Plusieurs carte possibles
//-Implémentation d'image lorsqu'on meurt/met en pause  (SEMI-DONE)
// Implémentation des musiques et bruitages (Exemple: quand une bombe explose)
// Amélioration du menu
// Prise en compte de la souris ?
// FullScreen pour jouer?