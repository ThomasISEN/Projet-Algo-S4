#include "../src/include/SDL2/SDL.h"
#include "../src/include/SDL2/SDL_image.h"
#include "../src/include/SDL2/SDL_ttf.h"
#include "../src/include/SDL2/SDL_mixer.h"
#include"affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//--------------------------------------------------------------------------------------------------------------
#define LARGEUR_TILE 50 // hauteur et largeur des tiles.
#define HAUTEUR_TILE 50
#define MUR 1
#define CHEMIN 0
#define ROCHER 2
#define BOMBE 3
SDL_Rect VecteurPositionPersonnage;

typedef struct BombeThread{
    //SDL_Window *fenetre;
    
    int map[13][13];
    int compteurBombe;
    
}BT;
BT Liste_Args;

//--------------------------------------------------------------------------------------------------------------
int menu(){
    

    
    VecteurPositionPersonnage.y=50;
    VecteurPositionPersonnage.x=50;
    VecteurPositionPersonnage.w=LARGEUR_TILE;
    VecteurPositionPersonnage.h=HAUTEUR_TILE;
    SDL_Window *fenetre=NULL;
    SDL_Renderer *renderer=NULL;
    int Carte[13][13];
    int COMPTEUR_BOMBE=0;
    if(TTF_Init()!=0){
        SDL_Log("Erreur: Initialisation TTF_Init > %s\n ",TTF_GetError());
        return EXIT_FAILURE;
    };
    TTF_Font *TestNouvellePartie=TTF_OpenFont("police/ChinaOne.ttf",30);

    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        SDL_Log("Erreur: Initialisation SDL_INIT_AUDIO > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    };
    
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024)!=0){
        SDL_Log("Erreur: Initialisation audio mix_init > %s\n ",Mix_GetError());
        return EXIT_FAILURE;
    }
    //Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    Mix_Music *MusiqueEntree;
    MusiqueEntree=Mix_LoadMUS("musique/KungFuPanda1.mp3");
    

    if(TestNouvellePartie==NULL){
        SDL_Log("Erreur: Initialisation TestNouvellePartie > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        SDL_Log("Erreur: Initialisation SDL > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    };
    
    //execution du programme
    fenetre=SDL_CreateWindow("Projet Bomberman",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,13*HAUTEUR_TILE,13*LARGEUR_TILE,0);
    if(fenetre==NULL){
        SDL_Log("Erreur: Création fenetre > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    /////////////////////////////////////
    renderer=SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_SOFTWARE);
    if(renderer==NULL){
        SDL_Log("Erreur: Création rendu > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderPresent(renderer);

    SDL_Surface *image=NULL;
    SDL_Texture *texture=NULL;
    SDL_Color rouge={255,0,0};
    
   

    image=SDL_LoadBMP("img/Maitre_Singe.bmp");

    SDL_Surface *EcritureNouvellePartie=TTF_RenderText_Solid(TestNouvellePartie,"Appuie sur 1 pour jouez une partie",rouge);

    if(image==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création Image > %s\n ",SDL_GetError());
        return EXIT_FAILURE;

    }
    if(EcritureNouvellePartie==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création EcritureNouvellePartie > %s\n ",SDL_GetError());
        return EXIT_FAILURE;

    }

    texture=SDL_CreateTextureFromSurface(renderer,image);
    SDL_Texture *TexturePourEcriture=SDL_CreateTextureFromSurface(renderer,EcritureNouvellePartie);

    SDL_FreeSurface(EcritureNouvellePartie);

    SDL_FreeSurface(image);
    if(texture==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création Texture > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    if(TexturePourEcriture==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création TexturePourEcriture > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Rect rectangle;
    SDL_Rect PositionEcriture; //Pour l'écriture

    if(SDL_QueryTexture(texture,NULL,NULL,&rectangle.w,&rectangle.h)!=0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création Rectangle+Texture > %s\n ",SDL_GetError());
        return EXIT_FAILURE;

    }
        if(SDL_QueryTexture(TexturePourEcriture,NULL,NULL,&PositionEcriture.w,&PositionEcriture.h)!=0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: Création Rectangle+Texture > %s\n ",SDL_GetError());
        return EXIT_FAILURE;

    }


    rectangle.x=(800-rectangle.w)/2;
    rectangle.y=(600-rectangle.w)/2;
    PositionEcriture.x=(800-PositionEcriture.w)/2;
    PositionEcriture.y=(1050-PositionEcriture.h)/2;

    if(SDL_RenderCopy(renderer,texture,NULL,&rectangle)!=0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: RenderCopy1 > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    
    if(SDL_RenderCopy(renderer,TexturePourEcriture,NULL,&PositionEcriture)!=0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
            SDL_Quit();
            SDL_Log("Erreur: RenderCopy2 > %s\n ",SDL_GetError());
        return EXIT_FAILURE;
    }
    //if(SDL_RenderClear(renderer)!=0){
        //SDL_Log("Erreur: Effacement rendu > %s\n ",SDL_GetError());
        //return EXIT_FAILURE;
    //}
    SDL_bool programmeEnCours=SDL_TRUE;
    Mix_PlayMusic(MusiqueEntree,-1);
    while(programmeEnCours){// boucle infinie pour le lancement de programme

        SDL_RenderPresent(renderer);
        SDL_Event evenement;
        while(SDL_PollEvent(&evenement)){//Jusqu'à ce qu'il se soit occupé de tous les évènements
            switch(evenement.type){//les differentes actions possibles
                case SDL_QUIT:
                    programmeEnCours=SDL_FALSE;
                    break;
                case SDL_KEYDOWN://appuie sur une touche
                    switch(evenement.key.keysym.sym){
                        case SDLK_SPACE://espace on ferme la fenetre
                            programmeEnCours=SDL_FALSE;
                            Mix_CloseAudio();
                            break;
                        case SDLK_1:
                           // printf("Appuie sur 1");
                            CreationMap(renderer,Carte);
                            InitialisationJoeur(renderer,Carte);
                            //menu(fenetre, renderer, image);
                            break;
                        case SDLK_z: //valeur 122
                            //printf("Appuie sur z");
                            DeplacementPersonnage(renderer,122,Carte);
                            break;
                        case SDLK_q: //valeur 113
                           // printf("Appuie sur q");
                            DeplacementPersonnage(renderer,113,Carte);
                            break;
                        case SDLK_s: //valeur 115
                           // printf("Appuie sur s");
                            DeplacementPersonnage(renderer,115,Carte);
                            break;
                        case SDLK_d:
                           // printf("Appuie sur d");
                            DeplacementPersonnage(renderer,SDLK_d,Carte);
                            break;
                        case SDLK_e:
                            printf("Depot de bombe");
                            
                            creation_bombe(COMPTEUR_BOMBE,Carte);
                            //printf("Valeur Liste %d \n",Liste_Args.map[0][0]);
                            //printf("Valeur carte solo %d \n",Carte[0][0]);
                            for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    Carte[i][j]=Liste_Args.map[i][j];
                                }
                            }
                            RefreshEcran(Carte,renderer);
                            break;
                        default:
                            break;
                        
                    }
                default:
                    break;
            }

        }
    }
    //SDL_RenderPresent(renderer);
    //SDL_Delay(3000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(TestNouvellePartie);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------
void creation_bombe(int compB,int map[13][13]){
     pthread_t AffichageBombe;
     //Liste_Args.map=map;
     Liste_Args.compteurBombe=2;
     //Liste_Args.fenetre=ecran;
     for(int i=0;i<13;i++){
        for(int j=0;j<13;j++){
            Liste_Args.map[i][j]=map[i][j];
        }
     }
     pthread_create(&AffichageBombe,NULL,gestion_bombe,&Liste_Args);
     pthread_join(AffichageBombe,NULL);
     //pthread_exit(NULL);
     //printf("Valeur apres creation_bombe %d \n",Liste_Args.compteurBombe);

}

void* gestion_bombe(void* arg){
    BT* liste_args=arg;
    liste_args->compteurBombe++;
    
   // int compteur_bombe=0;
    printf("La bombe va explose");
    
  
   
    //printf("%d\n",liste_args->compteurBombe);

    int COX_Perso=VecteurPositionPersonnage.x/50;
    int COY_Perso=VecteurPositionPersonnage.y/50;
    //printf("Coo au pif : %d \n",liste_args->map[0][0]);
    //printf("Valeur CoX, CoY : %d %d ",COX_Perso,COY_Perso);
    if(liste_args->map[COX_Perso][COY_Perso]==CHEMIN){
        printf("Depot de bombe possible\n");
        if(liste_args->map[COX_Perso-1][COY_Perso]==ROCHER){
            liste_args->map[COX_Perso-1][COY_Perso]=CHEMIN;
        }
        if(liste_args->map[COX_Perso][COY_Perso-1]==ROCHER){
            liste_args->map[COX_Perso][COY_Perso-1]=CHEMIN;
        }
        if(liste_args->map[COX_Perso][COY_Perso+1]==ROCHER){
            liste_args->map[COX_Perso][COY_Perso+1]=CHEMIN;
        }
        if(liste_args->map[COX_Perso+1][COY_Perso]==ROCHER){
            liste_args->map[COX_Perso+1][COY_Perso]=CHEMIN;
        }

    }
    else{
        printf("Impossible de pose la bombe \n");
        return liste_args;
    }
     Sleep(2000);
    return liste_args;
}

void *testPthread(void* StructureInfo){///////////////////////////////////////////////////////////////////////////////

    
    printf("C'est le pthread \n");
    
    
}

void CreationMap(SDL_Renderer* rendu, int map[13][13]){
    SDL_RenderClear(rendu);
    SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichageTiles=IMG_Load("img/Bamboo-Bloc.png");
    SDL_Surface* AffichageMurs=IMG_Load("img/Rock-Bloc.png");
    SDL_Surface* AffichageChemin=IMG_Load("img/Chemin.png");
    SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
    SDL_Texture* texture=NULL;
    SDL_Texture* texture2=NULL;
    SDL_Texture* texture3=NULL;
    SDL_Texture* texture4=NULL;
    texture=SDL_CreateTextureFromSurface(rendu,AffichageTiles);
    texture2=SDL_CreateTextureFromSurface(rendu,AffichageMurs);
    texture3=SDL_CreateTextureFromSurface(rendu,AffichageChemin);
    texture4=SDL_CreateTextureFromSurface(rendu,AffichageBomb);
    //int map[13][13];
   
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;
    printf("Creation Map");
    for(int i=0;i<13;i++){// J est un ligne, I est une colonne
        for(int j=0;j<13;j++){
            //printf("Case %d/%d \n",i,j);
           
            if(j%2!=0 && i%2==0 &&j>0 && i>0 &&j<12 && i<12){
                //printf("Debug");
                map[i][j]=ROCHER;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }
            else if(j%2==0 && i%2!=0 &&j>0 && i>0 &&j<12 && i<12) {
                //printf("Debug");
                map[i][j]=ROCHER;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }  
            else if(j%2==0 && i%2==0 &&j>0 && i>0 &&j<12 && i<12){
                 //printf("Debug");
                 map[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }  
            else if(j%2!=0 && i%2!=0 &&j>0 && i>0 &&j<12 && i<12){
                 //printf("Debug");
                 map[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else{
                map[i][j]=MUR;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition);
            }
            
            
        }
    }
    SDL_RenderPresent(rendu);
   
    //SDL_Delay(6000);
}

//--------------------------------------------------------------------------------------------------------------
void InitialisationJoeur(SDL_Renderer* rendu, int map[13][13]){
    SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Po-PD.png");
    SDL_Texture* Personnage1=NULL;
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    VecteurPosition.x=LARGEUR_TILE;
    VecteurPosition.y=HAUTEUR_TILE;
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPosition);
}

//--------------------------------------------------------------------------------------------------------------
void DeplacementPersonnage(SDL_Renderer* rendu,int touche,int Carte[13][13]){
   
    
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Po-PD.png");
    SDL_Texture* Personnage1=NULL;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    if (touche==122){
        int res = blocage(Carte,touche);
        if(res==0){
            RefreshEcran(Carte,rendu );
            VecteurPositionPersonnage.y=VecteurPositionPersonnage.y-HAUTEUR_TILE;
            printf("Deplacement vers le haut\n");
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
    else if(touche==113){
        int res = blocage(Carte,touche);
        if(res==0){
            RefreshEcran(Carte,rendu );
            VecteurPositionPersonnage.x=VecteurPositionPersonnage.x-LARGEUR_TILE;
            printf("Deplacement vers le gauche\n");
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }

    else if(touche==115){
        int res = blocage(Carte,touche);
        if(res==0){
            RefreshEcran(Carte,rendu );
            VecteurPositionPersonnage.y=VecteurPositionPersonnage.y+HAUTEUR_TILE;
            printf("Deplacement vers le bas\n");
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }

    }
    else{
        int res = blocage(Carte,touche);
        if(res==0){
            RefreshEcran(Carte,rendu);
            VecteurPositionPersonnage.x=VecteurPositionPersonnage.x+LARGEUR_TILE;
            printf("Deplacement vers la droite\n");
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
     
}

//--------------------------------------------------------------------------------------------------------------

int blocage(int map[13][13],int touche){

    int V1=0;
    int CoX=VecteurPositionPersonnage.x/50;
    int CoY=VecteurPositionPersonnage.y/50;

    if(touche==122){
        V1=map[CoX][CoY-1];
        if(V1==MUR){
            printf("bloquer haut\n");
            return -1;
        }
        return 0;
    }
    if(touche==115){
        V1=map[CoX][CoY+1];
        if(V1==MUR){
            printf("bloquer bas\n");
            return -1;
        }
        return 0;
    }
    if(touche==113){
        V1=map[CoX-1][CoY];
        if(V1==MUR){
            printf("bloquer gauche\n");
            return -1;
        }
        return 0;
    }
    if(touche==100){
        V1=map[CoX+1][CoY];
        if(V1==MUR){
            printf("bloquer droite\n");
            return -1;
        }
        return 0;
    }
    
}
//--------------------------------------------------------------------------------------------------------------
void RefreshEcran(int map[13][13],SDL_Renderer* rendu ){
    SDL_RenderClear(rendu);
    SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichageTiles=IMG_Load("img/Bamboo-Bloc.png");
    SDL_Surface* AffichageMurs=IMG_Load("img/Rock-Bloc.png");
    SDL_Surface* AffichageChemin=IMG_Load("img/Chemin.png");
    SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
    SDL_Texture* texture=NULL;
    SDL_Texture* texture2=NULL;
    SDL_Texture* texture3=NULL;
    SDL_Texture* texture4=NULL;
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;
    texture=SDL_CreateTextureFromSurface(rendu,AffichageTiles);
    texture2=SDL_CreateTextureFromSurface(rendu,AffichageMurs);
    texture3=SDL_CreateTextureFromSurface(rendu,AffichageChemin);
    texture4=SDL_CreateTextureFromSurface(rendu,AffichageBomb);
    printf("Debug Refresh");
        for(int i=0;i<13;i++){
            for(int j=0;j<13;j++){
            VecteurPosition.x=i*LARGEUR_TILE;
            VecteurPosition.y=j*HAUTEUR_TILE;
            if(map[i][j]==CHEMIN){
                 SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if(map[i][j]==MUR){
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition);
            }
            else if(map[i][j]==ROCHER){
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }
            else if(map[i][j]==BOMBE){
                SDL_RenderCopy(rendu,texture4,NULL,&VecteurPosition);
            }

        }
    }
    SDL_RenderPresent(rendu);
}
//--------------------------------------------------------------------------------------------------------------


int ExplosionInGame(int map[13][13],SDL_Renderer* rendu,int compteur_bombe){
    printf("La bombe va explose");
    SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
    SDL_Texture* texture=NULL;
    
    if(compteur_bombe!=0){
        printf("Bombe deja actif \n");
    }
    else{
        texture=SDL_CreateTextureFromSurface(rendu,AffichageBomb);
        SDL_RenderCopy(rendu, texture, NULL, &VecteurPositionPersonnage);
        int i=VecteurPositionPersonnage.x/50;
        int j=VecteurPositionPersonnage.y/50;
        map[i][j]=BOMBE;
        RefreshEcran(map,rendu);
        SDL_RenderPresent(rendu);
        compteur_bombe++;
        
    }
    //Sleep(1000);
    return compteur_bombe;
   
    
    //SDL_Delay(1000);
    //printf("Bombe Fini");
}
