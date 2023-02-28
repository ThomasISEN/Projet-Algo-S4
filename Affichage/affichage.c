#include "../src/include/SDL2/SDL.h"
#include "../src/include/SDL2/SDL_image.h"
#include "../src/include/SDL2/SDL_ttf.h"
#include "../src/include/SDL2/SDL_mixer.h"
#include"affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<string.h>


//--------------------------------------------------------------------------------------------------------------
#define LARGEUR_TILE 50 // hauteur et largeur des tiles.
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

typedef struct BombeThread{
    //SDL_Window *fenetre;
    
    int map[13][13];
    int compteurBombe;
    SDL_Renderer* rendu;
    
}BT;
BT Liste_Args;
int ETAT=0;
int Carte[13][13];
SDL_Renderer *renderer=NULL;

SDL_Window *fenetre=NULL;
int compteurBombe=0;
int JeuxFini=0;
    int choixX=0;
   
    int choixY=0;
int PowerUpP1POW=0;
int PowerUpP2POW=0;
 SDL_TimerID timerID;

 //-----------------------------------------------------------------------------------
 SDL_Surface* AffichagePersonnage2;
    SDL_Texture* Personnage2=NULL; 
    SDL_Surface* AffichagePersonnage;
    SDL_Texture* Personnage1=NULL;
SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichageTiles;
      SDL_Surface* AffichageMurs;
    SDL_Surface* AffichageChemin;
    SDL_Surface* AffichageBomb;
     SDL_Surface* AffichageBombExplosion;

 SDL_Texture* texture=NULL;
    SDL_Texture* texture2=NULL;
    SDL_Texture* texture3=NULL;
    SDL_Texture* texture4=NULL;
    SDL_Texture* texture5=NULL;
    //SDL_Texture* texture6=NULL;

    SDL_Surface* AffichageWin;
    SDL_Texture* Win=NULL;

    SDL_Surface* AffichageLoose;
    SDL_Texture* Loose=NULL; 
//--------------------------------------------------------------------------------------------------------------
int menu(){
    
     choixX=rand()%12 +1;
      choixY=rand()%12 +1;
    printf("Un PowerUp est apparu en [%d][%d] \n",choixX,choixY);
   
    
    
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
   // SDL_Texture *texture=NULL;
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
     /////////////////////////////////////////////////////////////////////////////////////////
    AffichagePersonnage2=IMG_Load("img/Maitre_Singe.bmp");
    AffichagePersonnage=IMG_Load("img/Po-PD.png");
    AffichageTiles=IMG_Load("img/Bamboo-Bloc.png");
    AffichageMurs=IMG_Load("img/Rock-Bloc.png");
    AffichageChemin=IMG_Load("img/Chemin.png");
    AffichageBomb=IMG_Load("img/bomb.png");
    AffichageBombExplosion=IMG_Load("img/explosion_bombe.jpg");
    AffichageLoose=IMG_Load("img/gameover.jpg");
     AffichageWin=IMG_Load("img/youwin.png");
    printf("Loading Image \n");


    texture=SDL_CreateTextureFromSurface(renderer,AffichageTiles);
    texture2=SDL_CreateTextureFromSurface(renderer,AffichageMurs);
    texture3=SDL_CreateTextureFromSurface(renderer,AffichageChemin);
    texture4=SDL_CreateTextureFromSurface(renderer,AffichageBomb);
    texture5=SDL_CreateTextureFromSurface(renderer,AffichageBombExplosion);
   // texture6=SDL_CreateTextureFromSurface(rendu,AffichageP1);
    Win=SDL_CreateTextureFromSurface(renderer,AffichageWin);
    Loose=SDL_CreateTextureFromSurface(renderer,AffichageLoose);
    Personnage2=SDL_CreateTextureFromSurface(renderer,AffichagePersonnage2);

    
    Personnage1=SDL_CreateTextureFromSurface(renderer,AffichagePersonnage);

    ////////////////////////////////////
    Liste_Args.rendu=SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_SOFTWARE);
    Liste_Args.compteurBombe=0;
    VecteurPositionPersonnage.y=50;
    VecteurPositionPersonnage.x=50;
    VecteurPositionPersonnage.w=LARGEUR_TILE;
    VecteurPositionPersonnage.h=HAUTEUR_TILE;
    VecteurPositionPersonnage2.y=550;
    VecteurPositionPersonnage2.x=550;
    VecteurPositionPersonnage2.w=LARGEUR_TILE;
    VecteurPositionPersonnage2.h=HAUTEUR_TILE;

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
                            InitialisationJoeur2(renderer,Carte);
                            ETAT=1;
                           // Liste_Args.rendu=renderer;
                            //menu(fenetre, renderer, image);
                            break;
                        case SDLK_z: //valeur 122
                            //printf("Appuie sur z");
                            printf("Valeur qui bug (Dans le deplacement avant le refresh) : %d \n",Carte[10][11]);
                            DeplacementPersonnage(renderer,122,Carte);
                            //Liste_Args.rendu=renderer;
                            //RefreshEcran(Carte,renderer);
                           // SauvegardeGame(Carte);
                            break;
                        case SDLK_q: //valeur 113
                           // printf("Appuie sur q");
                            DeplacementPersonnage(renderer,113,Carte);
                            //Liste_Args.rendu=renderer;
                           // RefreshEcran(Carte,renderer);
                             //SauvegardeGame(Carte);
                            break;
                        case SDLK_s: //valeur 115
                           // printf("Appuie sur s");
                            DeplacementPersonnage(renderer,115,Carte);
                            //Liste_Args.rendu=renderer;
                            //RefreshEcran(Carte,renderer);
                            //SauvegardeGame(Carte);
                            break;
                        case SDLK_d:
                           // printf("Appuie sur d");
                            DeplacementPersonnage(renderer,SDLK_d,Carte);
                            //Liste_Args.rendu=renderer;
                           // RefreshEcran(Carte,renderer);
                           // SauvegardeGame(Carte);
                            break;
                        case SDLK_e:
                            printf("Depot de bombe");
                            //Liste_Args.rendu=renderer;
                            if(Liste_Args.compteurBombe==0){
                                creation_bombe(COMPTEUR_BOMBE,Carte,renderer);
                                 //ConditionVictoire(Carte,1,VecteurPositionPersonnage.x,VecteurPositionPersonnage.y);
                            }
                            else{
                                printf("Impossible Bombe explosion \n");
                            }
                            
                        //    // printf("Valeur Liste chemin %d \n",Liste_Args.map[2][2]);
                        //     printf("Valeur Liste rocher %d \n",Liste_Args.map[2][1]);
                        //     printf("Valeur carte chemin %d \n",Carte[2][2]);
                        //     printf("Valeur carte rocher %d \n",Carte[2][1]);
                            //printf("Valeur carte solo %d \n",Carte[0][0]);
                            
                            
                            //RefreshEcran(Carte,renderer);
                            break;
                        case SDLK_t:
                            // printf("Valeur Liste chemin %d \n",Liste_Args.map[2][2]);
                            // printf("Valeur Liste rocher %d \n",Liste_Args.map[2][1]);
                            SauvegardeGame(Carte);
                            break;
                            
                        case SDLK_r:
                            printf("Chargement en Cours \n");
                            
                            ChargementGame(Carte);
                           // Sleep(1000);
                            // for(int i=0;i<13;i++){
                            //     for(int j=0;j<13;j++){
                            //         printf("Valeur Lecture map apres chargement %d \n",Carte[i][j]);
                            //         //Carte[i][j]=Liste_Args.map[i][j];
                            //         //printf("Valeur carte %d \n",Carte[i][j]);
                            //     }
                            // }
                            //RefreshEcran(Carte,renderer);
                            //printf("Refresh ecran FINITO \n");
                            break;
                        case SDLK_o: //111
                            printf("Deplacement perso 2 \n");
                            DeplacementPersonnage2(renderer,SDLK_o,Carte);
                           // RefreshEcran(Carte,renderer);
                            break;
                        case SDLK_m: //109
                            printf("Deplacement perso 2 \n");
                            DeplacementPersonnage2(renderer,SDLK_m,Carte);
                           // RefreshEcran(Carte,renderer);
                            break;
                        case SDLK_l: //108
                            printf("Deplacement perso 2 \n");
                            DeplacementPersonnage2(renderer,SDLK_l,Carte);
                           // RefreshEcran(Carte,renderer);
                            break;
                        case SDLK_k: //107
                            printf("Deplacement perso 2 \n");
                            DeplacementPersonnage2(renderer,SDLK_k,Carte);
                           // RefreshEcran(Carte,renderer);
                            break;
                        case SDLK_p:
                            printf("Depot de bombe");
                            Liste_Args.rendu=renderer;
                            if(compteurBombe==0){
                                creation_bombe2(COMPTEUR_BOMBE,Carte,renderer);
                                //ConditionVictoire(Carte,2,VecteurPositionPersonnage2.x,VecteurPositionPersonnage2.y);
                            }
                            else{
                                printf("Impossible Bombe explosion \n");
                            }
                           // RefreshEcran(Carte,renderer);
                            break;
                        default:
                            //RefreshEcran(Carte,renderer);
                            break;
                        
                    }
                    
                default:
                    //RefreshEcran(Carte,renderer);
                    break;
            //RefreshEcran(Carte,renderer);
            }
            
        }
        if(ETAT==1){
            frame_rate(24);
        }
        
        //RefreshEcran(Carte,renderer);
    }
    //SDL_RenderPresent(renderer);
    //SDL_Delay(3000);
    printf("Fin de programme \n");
    SDL_RemoveTimer( timerID );
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(TestNouvellePartie);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------
void frame_rate(int fps) {
    static Uint32 prev_ticks = 0;
    Uint32 ticks = SDL_GetTicks();
    Uint32 min_ticks = 1000 / fps;
    if (ticks - prev_ticks < min_ticks) {
        SDL_Delay(min_ticks - (ticks - prev_ticks));
         RefreshEcran(Carte,renderer);
    }
    prev_ticks = SDL_GetTicks();
   
}




void creation_bombe(int compB,int map[13][13],SDL_Renderer* rendu){
     pthread_t AffichageBombe;
     //Liste_Args.map=map;
     //Liste_Args.compteurBombe=2;
     //Liste_Args.fenetre=ecran;
     for(int i=0;i<13;i++){
        for(int j=0;j<13;j++){
            Liste_Args.map[i][j]=map[i][j];
        }
     }
     pthread_create(&AffichageBombe,NULL,gestion_bombe,&Liste_Args);
     // timerID= SDL_AddTimer(2000,gestion_bombe_test,NULL);
    // pthread_join(AffichageBombe,NULL);  //problème block le main si présent mais sans , aucun return sur la structure
     //;
     //printf("Valeur apres creation_bombe %d \n",Liste_Args.compteurBombe);
     printf("Fin du pthread_create \n");
    // Sleep(2000);
     //SDL_RemoveTimer( timerID );
    //RefreshEcran(Carte,rendu);

}
Uint32 gestion_bombe_test(){
    printf("Add Timer \n");
    int coX=VecteurPositionPersonnage.x/50;
    int coY=VecteurPositionPersonnage.y/50;
    Carte[coX][coY]=BOMBE;
   // RefreshEcran(Carte,renderer);
    return 1;
}
void creation_bombe2(int compB,int map[13][13],SDL_Renderer* rendu){
     pthread_t AffichageBombe;
     //Liste_Args.map=map;
     //Liste_Args.compteurBombe=2;
     //Liste_Args.fenetre=ecran;
     for(int i=0;i<13;i++){
        for(int j=0;j<13;j++){
            Liste_Args.map[i][j]=map[i][j];
        }
     }
     pthread_create(&AffichageBombe,NULL,gestion_bombe2,&Liste_Args);
     //pthread_join(AffichageBombe,NULL);  //problème block le main si présent mais sans , aucun return sur la structure
     //;
     //printf("Valeur apres creation_bombe %d \n",Liste_Args.compteurBombe);
     printf("Fin du pthread_create \n");
     
    //RefreshEcran(Carte,rendu);

}
/* Fonction gestion_bombe2
 
    Paramètres: -void* arg -> C'est un pointeur vers un type void (tt les types), c'est en général, une structure avec tt les arguments 
                             à l'interieur
                
                

    But de la fonction : Cette fonction permet de gerer les bombes dans un thread autre que le main (sans blocker donc le thread principal)
 
 
 */

void* gestion_bombe2(void* arg){
    //SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
   // SDL_Texture* texture=NULL;
   // BT* liste_args=arg;
    //liste_args->compteurBombe++;
    
   // int compteur_bombe=0;
    printf("La bombe va explose");
    
   // extern Liste_Args
   
    //printf("%d\n",liste_args->compteurBombe);

    int COX_Perso=VecteurPositionPersonnage2.x/50;
    int COY_Perso=VecteurPositionPersonnage2.y/50;
    
        
    //printf("Coo au pif : %d \n",liste_args->map[0][0]);
    //printf("Valeur CoX, CoY : %d %d ",COX_Perso,COY_Perso);
    if(Carte[COX_Perso][COY_Perso]==CHEMIN){
        
       // printf("Depot de bombe possible\n");
        // SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
        // SDL_Texture* texture=NULL;
        printf("Valeur Bombe Liste_Args %d \n",compteurBombe);
        if(compteurBombe!=0){
            printf("Bombe deja actif \n");
            // int i=VecteurPositionPersonnage.x/50;
            // int j=VecteurPositionPersonnage.y/50;
            // Carte[i][j]=CHEMIN;
        }
        else{
            // texture=SDL_CreateTextureFromSurface(Liste_Args.rendu,AffichageBomb);
            // SDL_RenderCopy(Liste_Args.rendu, texture, NULL, &VecteurPositionPersonnage);
            int i=VecteurPositionPersonnage2.x/50;
            int j=VecteurPositionPersonnage2.y/50;
            Carte[i][j]=BOMBE;
            //RefreshEcran(Carte,renderer);
           // SDL_RenderPresent(Liste_Args.rendu);
            compteurBombe++;
            SDL_Delay(2000);
            ExplosionInGame(i,j);
            if(Carte[COX_Perso-1][COY_Perso]==BOMBE_EXPLOSION){
                //printf("Coo au pif Ancienne version: %d \n",liste_args->map[0][0]);
            
                
            
                
                Carte[COX_Perso-1][COY_Perso]=CHEMIN;
                //printf("Coo au pif New version: %d \n",liste_args->map[COX_Perso-1][COY_Perso]);
            }
            if(Carte[COX_Perso][COY_Perso-1]==BOMBE_EXPLOSION){
                
                Carte[COX_Perso][COY_Perso-1]=CHEMIN;
            }
            if(Carte[COX_Perso][COY_Perso+1]==BOMBE_EXPLOSION){
                Carte[COX_Perso][COY_Perso+1]=CHEMIN;
            }
            if(Carte[COX_Perso+1][COY_Perso]==BOMBE_EXPLOSION){
                Carte[COX_Perso+1][COY_Perso]=CHEMIN;
            }
            compteurBombe--;
            Carte[i][j]=CHEMIN;
        }
        

    }
    else{
        printf("Impossible de pose la bombe \n");
        //  int i=VecteurPositionPersonnage2.x/50;
        //     int j=VecteurPositionPersonnage2.y/50;
        // Carte[i][j]=CHEMIN;
        return NULL;
    }
     
    
    printf("La bombe a explose \n");
    
    // for(int i=0;i<13;i++){
    //                             for(int j=0;j<13;j++){
    //                                 //printf("Valeur Liste %d \n",Liste_Args.map[i][j]);
    //                                 Carte[i][j]=Liste_Args.map[i][j];
    //                                 //printf("Valeur carte %d \n",Carte[i][j]);
    //                             }
    //                         }
     ConditionVictoire(Carte,2,COX_Perso,COY_Perso);
    //  Carte[11][10]=WIN;
    //     Carte[10][11]=WIN;
     //Sleep(1000);
    // printf("Valeur Liste chemin in function %d \n",Liste_Args.map[2][2]);
    // printf("Valeur Liste rocher in function %d \n",Liste_Args.map[2][1]);
    // //pthread_join(AffichageBombe,NULL);
    //RefreshEcran(Carte,Liste_Args.rendu);
    
    return 0;
}

/* Fonction gestion_bombe
 
    Paramètres: -void* arg -> C'est un pointeur vers un type void (tt les types), c'est en général, une structure avec tt les arguments 
                             à l'interieur
                
                

    But de la fonction : Cette fonction permet de gerer les bombes dans un thread autre que le main (sans blocker donc le thread principal)
 
 
 */
void* gestion_bombe(void* arg){
   
    //SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
   // SDL_Texture* texture=NULL;
   // BT* liste_args=arg;
    //liste_args->compteurBombe++;
    
   // int compteur_bombe=0;
    printf("La bombe va explose");
    
   // extern Liste_Args
   
    //printf("%d\n",liste_args->compteurBombe);

    int COX_Perso=VecteurPositionPersonnage.x/50;
    int COY_Perso=VecteurPositionPersonnage.y/50;
    
        
    //printf("Coo au pif : %d \n",liste_args->map[0][0]);
    //printf("Valeur CoX, CoY : %d %d ",COX_Perso,COY_Perso);
    if(Liste_Args.map[COX_Perso][COY_Perso]==CHEMIN){
        
        // printf("Depot de bombe possible\n");
        // SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
        // SDL_Texture* texture=NULL;
        printf("Valeur Bombe Liste_Args %d \n",Liste_Args.compteurBombe);
        if(Liste_Args.compteurBombe!=0){
            printf("Bombe deja actif \n");
            // int i=VecteurPositionPersonnage.x/50;
            // int j=VecteurPositionPersonnage.y/50;
            // Carte[i][j]=CHEMIN;
        }
        else{
            // texture=SDL_CreateTextureFromSurface(Liste_Args.rendu,AffichageBomb);
            // SDL_RenderCopy(Liste_Args.rendu, texture, NULL, &VecteurPositionPersonnage);
            int i=VecteurPositionPersonnage.x/50;
            int j=VecteurPositionPersonnage.y/50;
            Carte[i][j]=BOMBE;
            //RefreshEcran(Carte,renderer2);
           // SDL_RenderPresent(Liste_Args.rendu);
            Liste_Args.compteurBombe++;
            SDL_Delay(2000);
           // RefreshEcran(Carte,Liste_Args.rendu);
            //printf("entre dans refresh ecran \n");
    
            //printf("Rendu FINI!!!!!!!!!!!!!!!!!! \n");
            ExplosionInGame(i,j); /* ICI ON DOIT RAFRAICHIR QUAND SA EXPLOSE*/
            //RefreshEcran(Carte,Liste_Args.rendu);
            /* Modification de la carte après l'explosion */
            if(Liste_Args.map[COX_Perso-1][COY_Perso]==ROCHER){
                //printf("Coo au pif Ancienne version: %d \n",liste_args->map[0][0]);
            
                
            
                
                Liste_Args.map[COX_Perso-1][COY_Perso]=CHEMIN;
                //printf("Coo au pif New version: %d \n",liste_args->map[COX_Perso-1][COY_Perso]);
            }
            if(Liste_Args.map[COX_Perso][COY_Perso-1]==ROCHER){
                
                Liste_Args.map[COX_Perso][COY_Perso-1]=CHEMIN;
            }
            if(Liste_Args.map[COX_Perso][COY_Perso+1]==ROCHER){
                Liste_Args.map[COX_Perso][COY_Perso+1]=CHEMIN;
            }
            if(Liste_Args.map[COX_Perso+1][COY_Perso]==ROCHER){
                Liste_Args.map[COX_Perso+1][COY_Perso]=CHEMIN;
            }
            Liste_Args.compteurBombe--;
            Carte[i][j]=CHEMIN;
        }
        

    }
    else{
        printf("Impossible de pose la bombe \n");
        //  int i=VecteurPositionPersonnage.x/50;
        //     int j=VecteurPositionPersonnage.y/50;
        // Carte[i][j]=CHEMIN;
        return NULL;
    }
     
    
    printf("La bombe a explose \n");
    /* On remet les valeurs dans la carte principal (par soucis de syncro)*/
    for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    //printf("Valeur Liste %d \n",Liste_Args.map[i][j]);
                                    Carte[i][j]=Liste_Args.map[i][j];
                                    //printf("Valeur carte %d \n",Carte[i][j]);
                                }
                            }
    /* On test si qq à gagner*/
    ConditionVictoire(Carte,1,COX_Perso,COY_Perso);
    
    //RefreshEcran(Carte,renderer2);
    // Carte[11][10]=WIN;
    // Carte[10][11]=WIN;
    // printf("Valeur Liste chemin in function %d \n",Liste_Args.map[2][2]);
    // printf("Valeur Liste rocher in function %d \n",Liste_Args.map[2][1]);
    // //pthread_join(AffichageBombe,NULL);
    //RefreshEcran(Carte,Liste_Args.rendu);
    
    return 0;
}

/* Fonction SauvegardeGame
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                
                

    But de la fonction : Cette fonction permet de sauvegarde une partie dans une sauvegarde en TXT, elle sauvegardera le statut de chaque case et
                         la position des joueurs
 
 
 */

void SauvegardeGame(int carte[13][13]){
    printf("Sauvegarde en cours \n");
    FILE *fp;
    char temp[10];
    /* On ouvre le fichier*/
	fp=fopen("sauvegarde.txt","w");
	if(fp==NULL){
        /* Cas où le fichier est illisible*/
		printf("erreur ouverture sauvegarde");
	}
	else{
		printf("fichier correctement ouvert/ecrit \n");
        
        for(int i=0;i<13;i++){
            for(int j=0;j<13;j++){
                sprintf(temp,"%d",carte[i][j]);
                fputs(temp,fp); /* on transforme le int en string et on le sauvegarde */
                //fputs(" ",fp); //separe les cases dans le fichies sauvegarde (utile pour charger ensuite une partie \n");
                
                
                
                
            }
	    }
        // fputs(" ",fp);
        // sprintf(temp,"%d",VecteurPositionPersonnage.x);
        // fputs(temp,fp);
        // fputs(" ",fp);
        // sprintf(temp,"%d",VecteurPositionPersonnage.y);
        // fputs(temp,fp);
        // fputs(" ",fp);
        // sprintf(temp,"%d",VecteurPositionPersonnage2.x);
        // fputs(temp,fp);
        // fputs(" ",fp);
        // sprintf(temp,"%d",VecteurPositionPersonnage2.y);
        // fputs(temp,fp);
	}
   
    fclose(fp);
}

/* Fonction ChargementGame
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                
                

    But de la fonction : Cette fonction permet de charger une partie depuis une sauvegarde en TXT, elle modifie ensuite la carte de jeu et
                         Affiche la modification
 
 
 */

void ChargementGame(int carte[13][13]){
    FILE *fp;
    /* On ouvre le fichier*/
	fp=fopen("sauvegarde.txt","r");
    printf("Valeur [%d]",VecteurPositionPersonnage.x);
     printf("Valeur [%d]",VecteurPositionPersonnage.y);
    char parcourt;
    /* Mes variables de parcourt de carte*/
    int i=0;
    int j=0;
    if(fp==NULL){
            /* Cas où le fichier est illisible*/
            printf("erreur ouverture sauvegarde");
        }
    else{
       // printf("Fichier correctement ouvert \n");
        while(parcourt!=EOF){ /* Tant que le fichier n'est pas fini*/
            //printf("Dans le while \n");
            parcourt=fgetc(fp);
           // printf("Lecture du fichier: %c\n",parcourt);
            if(parcourt=='0'){ /* On met la valeur à chemin*/
                carte[i][j]=CHEMIN;
               // printf("Chargement des chemins \n");
            }
            else if(parcourt=='1'){ /* idem avec mur*/
                carte[i][j]=MUR;
                //printf("Chargement des murs \n");
            }
            else if(parcourt=='2'){/* idem avec ROCHER*/
                carte[i][j]=ROCHER;
                //printf("Chargement des rochers \n");
            }
            else if(parcourt=='3'){/* idem avec BOMBE*/
                carte[i][j]=BOMBE;
                //printf("Chargement des bombes \n");
            }
            // else if(parcourt==' '){
            //     char temp;
            //     printf("Entre dans le parcourt \n");
            //     while(temp!=' '){
            //         temp=fgetc(fp);
            //     }
            //     printf("Affichage du temp %c \n",temp);
            //     //VecteurPositionPersonnage=itoa(temp);
                
            // }
            else{
                printf("Autre cas (espace) \n");
                //char temp;
                //char ValeurEnString[10];
                //printf("Entre dans le parcourt \n");
                // while(temp!=' '){
                //     temp=fgetc(fp);
                //     //strcat(ValeurEnString,temp);
                // }
                //printf("Affichage du temp: %s \n",ValeurEnString);
            }
            j++; // On change de case
            if(j==13){ // si la ligne est plein
                j=0; // on remet à 0 le j est on augment le i
                //printf("Remise a zero \n");
                i++;
            }
            
        }
        }
       // printf("Fermture du fichier \n");
    fclose(fp); /* Fermeture du fichier*/
}

/* Fonction CreationMap
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction initialise la carte de jeu et donne le bon statut aux cases de la matrice
 
 
 */


void CreationMap(SDL_Renderer* rendu, int map[13][13]){
    SDL_RenderClear(rendu);
    VecteurPositionPersonnage.y=50;
    VecteurPositionPersonnage.x=50;
    VecteurPositionPersonnage.w=LARGEUR_TILE;
    VecteurPositionPersonnage.h=HAUTEUR_TILE;
    VecteurPositionPersonnage2.y=550;
    VecteurPositionPersonnage2.x=550;
    VecteurPositionPersonnage2.w=LARGEUR_TILE;
    VecteurPositionPersonnage2.h=HAUTEUR_TILE;
    SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichageTiles=IMG_Load("img/Bamboo-Bloc.png");
    SDL_Surface* AffichageMurs=IMG_Load("img/Rock-Bloc.png");
    SDL_Surface* AffichageChemin=IMG_Load("img/Chemin.png");
   // SDL_Surface* AfficheP1=IMG_Load("Img/pow.png");
    //SDL_Surface* AffichageBomb=IMG_Load("img/bomb.png");
    SDL_Texture* texture=NULL;
    SDL_Texture* texture2=NULL;
    SDL_Texture* texture3=NULL;
   // SDL_Texture* texture4=NULL;
    //SDL_Texture* texture4=NULL;
    texture=SDL_CreateTextureFromSurface(rendu,AffichageTiles);
    texture2=SDL_CreateTextureFromSurface(rendu,AffichageMurs);
    texture3=SDL_CreateTextureFromSurface(rendu,AffichageChemin);
   // texture4=SDL_CreateTextureFromSurface(rendu,AfficheP1);
    //texture4=SDL_CreateTextureFromSurface(rendu,AffichageBomb);
    //int map[13][13];
   
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;
    printf("Creation Map");


       


    for(int i=0;i<13;i++){// J est un ligne, I est une colonne
        for(int j=0;j<13;j++){
            //printf("Case %d/%d \n",i,j);
           
            if(j%2!=0 && i%2==0 &&j>0 && i>0 &&j<12 && i<12){
                //printf("Debug");
                Carte[i][j]=ROCHER;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }
            // else if(i==choixX && j==choixY){
            //     Carte[i][j]=POW;
            //      VecteurPosition.x=i*LARGEUR_TILE;
            //     VecteurPosition.y=j*HAUTEUR_TILE;
            //     SDL_RenderCopy(rendu, texture4, NULL, &VecteurPosition);
            // }
            else if(i==11 && j==10){
                Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if((i==10 && j==11)){
                Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if((i==1 && j==2)){
                Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if((i==2 && j==1)){
                Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else if(j%2==0 && i%2!=0 &&j>0 && i>0 &&j<12 && i<12) {
                //printf("Debug");
                Carte[i][j]=ROCHER;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);
            }  
            else if(j%2==0 && i%2==0 &&j>0 && i>0 &&j<12 && i<12){
                 //printf("Debug");
                 Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }  
            else if(j%2!=0 && i%2!=0 &&j>0 && i>0 &&j<12 && i<12){
                 //printf("Debug");
                 Carte[i][j]=CHEMIN;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
            }
            else{
                Carte[i][j]=MUR;
                VecteurPosition.x=i*LARGEUR_TILE;
                VecteurPosition.y=j*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition);
            }
            
            
        }
       
    }
   
    // Carte[1][2]=CHEMIN;
    // VecteurPosition.x=1*LARGEUR_TILE;
    // VecteurPosition.y=2*HAUTEUR_TILE;
    // SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
    // Carte[2][1]=CHEMIN;
    //  VecteurPosition.x=2*LARGEUR_TILE;
    // VecteurPosition.y=1*HAUTEUR_TILE;
    // SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
    // Carte[11][10]=CHEMIN;
    //  VecteurPosition.x=11*LARGEUR_TILE;
    // VecteurPosition.y=10*HAUTEUR_TILE;
    // SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
    // Carte[10][11]=CHEMIN;
    //  VecteurPosition.x=10*LARGEUR_TILE;
    // VecteurPosition.y=11*HAUTEUR_TILE;
    // SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition);
    SDL_RenderPresent(rendu);
    //Carte[10][11]=LOOSE;
    //RefreshEcran(Carte,rendu);
   
    //SDL_Delay(6000);
    //return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/* Fonction InitialisationJoeur
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction initialise la texture des joueurs sur la carte pour la première fois et affiche la modification
 
 
 */

void InitialisationJoeur(SDL_Renderer* rendu, int map[13][13]){
    /* Chargement des textures*/
    SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Po-PD.png");
    SDL_Texture* Personnage1=NULL;
    /* Fin de chargement*/
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    VecteurPosition.x=LARGEUR_TILE;
    VecteurPosition.y=HAUTEUR_TILE;
    /* Affichage du personnage*/
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPosition);
}
/* Fonction InitialisationJoeur2
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction initialise la texture des joueurs sur la carte pour la première fois et affiche la modification
 
 
 */

void InitialisationJoeur2(SDL_Renderer* rendu, int map[13][13]){
   // SDL_Rect VecteurPosition; //Vecteur qui me permet de parcourir toute la fenetre avec des dimensions CONNUES
   /* Chargement des textures*/
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Maitre_Singe.bmp");
    SDL_Texture* Personnage1=NULL;
    //VecteurPosition.w=LARGEUR_TILE;
    //VecteurPosition.h=HAUTEUR_TILE;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    /* Fin de chargement*/
    //VecteurPosition.x=LARGEUR_TILE;
    //VecteurPosition.y=HAUTEUR_TILE;

    /* Affichage du personnage*/
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
}

//--------------------------------------------------------------------------------------------------------------
/* Fonction DéplacementPersonnage
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -int touche -> La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction permet d'effectuer le déplacement du personnage en fonction de la direction 
                         du déplacement (touche), elle appelera la fonction blocage pour savoir si le déplacement est possible
                         On affiche ensuite sur la fenetre de jeu , la nouvelle position du personnage après déplacement
 
 
 */

void DeplacementPersonnage(SDL_Renderer* rendu,int touche,int Carte[13][13]){
   
    /* Chargement des textures*/
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Po-PD.png");
    SDL_Texture* Personnage1=NULL;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    /* Fin de chargement */


    /* On test la touche appuyé (si res est différent de 0 , alors le déplacement est impossible)*/
    if (touche==122){
        int res = blocage(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu );
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage.y=VecteurPositionPersonnage.y-HAUTEUR_TILE;
            printf("Deplacement vers le haut\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
    else if(touche==113){
        int res = blocage(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu );
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage.x=VecteurPositionPersonnage.x-LARGEUR_TILE;
            printf("Deplacement vers le gauche\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }

    else if(touche==115){
        int res = blocage(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu );
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage.y=VecteurPositionPersonnage.y+HAUTEUR_TILE;
            printf("Deplacement vers le bas\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }

    }
    else{
        int res = blocage(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu);
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage.x=VecteurPositionPersonnage.x+LARGEUR_TILE;
            printf("Deplacement vers la droite\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
            SDL_RenderPresent(rendu);
        }
    }
     int test=0;
     test=VecteurPositionPersonnage.x/50;
     int test2=0;
     test2=VecteurPositionPersonnage.y/50;
     printf("Perso en [%d][%d] \n",test,test2);
     if(test==choixX && test2==choixY){
        printf("augmentation de la taille de bombe \n");
        PowerUpP1POW=1;
     }
     else{
        printf("Pas de PowerUp \n");
     }

}
/* Fonction DéplacementPersonnage2
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -int touche -> La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction permet d'effectuer le déplacement du personnage2 en fonction de la direction 
                         du déplacement (touche), elle appelera la fonction blocage2 pour savoir si le déplacement est possible
                         On affiche ensuite sur la fenetre de jeu , la nouvelle position du personnage après déplacement
 
 
 */

void DeplacementPersonnage2(SDL_Renderer* rendu,int touche,int Carte[13][13]){
   
    /* Chargement des textures*/
    SDL_Surface* AffichagePersonnage=IMG_Load("img/Maitre_Singe.bmp");
    SDL_Texture* Personnage1=NULL;
    Personnage1=SDL_CreateTextureFromSurface(rendu,AffichagePersonnage);
    /* Fin de chargement */

    /* On test la touche appuyé (si res est différent de 0 , alors le déplacement est impossible)*/
    if (touche==SDLK_o){
        int res = blocage2(Carte,touche);
        if(res==0){
           // RefreshEcran(Carte,rendu );
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage2.y=VecteurPositionPersonnage2.y-HAUTEUR_TILE;
            printf("Deplacement vers le haut\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }
    else if(touche==SDLK_k){
        int res = blocage2(Carte,touche);
        printf("Test");
        if(res==0){
            //RefreshEcran(Carte,rendu );
            printf("Test dans le res");
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage2.x=VecteurPositionPersonnage2.x-LARGEUR_TILE;
            printf("Deplacement vers le gauche\n");
            printf("%d %d ",VecteurPositionPersonnage2.x,VecteurPositionPersonnage2.y);
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }

    else if(touche==SDLK_l){
        int res = blocage2(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu );
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage2.y=VecteurPositionPersonnage2.y+HAUTEUR_TILE;
            printf("Deplacement vers le bas\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }

    }
    else{
        int res = blocage2(Carte,touche);
        if(res==0){
            //RefreshEcran(Carte,rendu);
            /* On modifie son VecteurPosition en fonction de la touche*/
            VecteurPositionPersonnage2.x=VecteurPositionPersonnage2.x+LARGEUR_TILE;
            printf("Deplacement vers la droite\n");
            /* On affiche la modification*/
            SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage2);
            SDL_RenderPresent(rendu);
        }
    }
     
}

//--------------------------------------------------------------------------------------------------------------
/* Fonction Blocage2
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -int touche -> La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
                

    But de la fonction : Cette fonction permet de tester si le déplacement du personnage2 est possible en fonction de la direction 
                         du mouvement (touche) et également de la case visé par ce déplacement 
                         Le déplacement est seulement possible si la case où l'on veut se déplacer est un chemin
 
 
 */

int blocage2(int map[13][13],int touche){

    int V1=0;
    /* On récupère les coordonées actuels du personnage*/
    int CoX=VecteurPositionPersonnage2.x/50;
    int CoY=VecteurPositionPersonnage2.y/50;
    printf("%d %d ",CoX,CoY);
    /* on test en fonction du déplacement, si la case est libre*/
    if(touche==SDLK_o){
        V1=map[CoX][CoY-1];
        if(V1==MUR || V1==ROCHER){
            printf("bloquer haut\n");
            return -1;
        }
        return 0;
    }
    if(touche==SDLK_l){
        V1=map[CoX][CoY+1];
        if(V1==MUR || V1==ROCHER){
            printf("bloquer bas\n");
            return -1;
        }
        return 0;
    }
    if(touche==SDLK_k){
        V1=map[CoX-1][CoY];
        if(V1==MUR || V1==ROCHER){
            printf("bloquer gauche\n");
            return -1;
        }
        return 0;
    }
    if(touche==SDLK_m){
        V1=map[CoX+1][CoY];
        if(V1==MUR || V1==ROCHER){
            printf("bloquer droite\n");
            return -1;
        }
        return 0;
    }
    return -1;
    /* return 0 indique que le déplacement est possible
       return -1 indique que le déplacement n'est pas possible ou qu'une erreur est apparu 
    */

}

/* Fonction Blocage
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -int touche -> La valeur en int (ou en SDLK_KEY) de la touche pressé , permet de savoir la direction du deplacement
                

    But de la fonction : Cette fonction permet de tester si le déplacement du personnage est possible en fonction de la direction 
                         du mouvement (touche) et également de la case visé par ce déplacement 
                         Le déplacement est seulement possible si la case où l'on veut se déplacer est un chemin
 
 
 */
int blocage(int map[13][13],int touche){

    int V1=0;
    /* On récupère les coordonées actuels du personnage*/
    int CoX=VecteurPositionPersonnage.x/50;
    int CoY=VecteurPositionPersonnage.y/50;

    /* on test en fonction du déplacement, si la case est libre*/
    if(touche==122){
        V1=map[CoX][CoY-1];
        if(V1==MUR /*|| V1==ROCHER*/){
            printf("bloquer haut\n");
            return -1;
        }
        return 0;
    }
    if(touche==115){
        V1=map[CoX][CoY+1];
        if(V1==MUR /*|| V1==ROCHER*/){
            printf("bloquer bas\n");
            return -1;
        }
        return 0;
    }
    if(touche==113){
        V1=map[CoX-1][CoY];
        if(V1==MUR /*|| V1==ROCHER*/){
            printf("bloquer gauche\n");
            return -1;
        }
        return 0;
    }
    if(touche==100){
        V1=map[CoX+1][CoY];
        if(V1==MUR /*|| V1==ROCHER*/){
            printf("bloquer droite\n");
            return -1;
        }
        return 0;
    }
    return -1;
    /* return 0 indique que le déplacement est possible
       return -1 indique que le déplacement n'est pas possible ou qu'une erreur est apparu 
    */
}
//--------------------------------------------------------------------------------------------------------------

/* Fonction RefreshEcran
 
    Paramètres: -int map[13][13] -> La matrice qui représente la carte de jeu
                -SDL_Renderer* rendu -> C'est un renderer SDL qui permet de copier les textures dedant et donc de faire l'affichage de celle-ci
                

    But de la fonction : Cette fonction modifie l'affichage de la fenetre en fonction de la carte de jeu
                         On recrée toute la fenetre à chaque appel et on test toutes les cases de la matrice pour copier
                         dans le rendu la bonne texture 
 
 
 */

void RefreshEcran(int map[13][13],SDL_Renderer* rendu){
    //printf("entre dans refresh ecran \n");
    /* On efface ce qui était présent dans le rendu (des résidus que l'on ne veut pas dans l'affichage)*/
   if(SDL_RenderClear(rendu)==0){
    //printf("Rendu ok \n");
   };

    /* Chargement de toutes les textures/images dont on a besoin pour la carte de jeu*/
    
    


    //printf("Rendu clear \n");
    
    if(AffichageTiles==NULL){
        printf("Erreur chargement Tiles \n");
    }
  
     //SDL_Surface* AffichageP1=IMG_Load("img/pow.png");
    //printf("Chargement des surfaces finies \n");
   

    /*Fin du chargement des textures*/
   // printf("Chargement des textures finies \n");
    /* On définit un VecteurPosition, son but est de "parcourir" toute la fenetre de jeu pour placer au bonne endroit les textures chargées*/
    VecteurPosition.w=LARGEUR_TILE;
    VecteurPosition.h=HAUTEUR_TILE;

    /*On creé depuis les surfaces chargées , des textures qui seront ensuite copiées dans le rendu */
    

    /* Fin de la création des textures*/

    //printf("Debug Refresh"); 

    /* On parcourt ensuite toute la carte */
        for(int i=0;i<13;i++){
            for(int j=0;j<13;j++){
            VecteurPosition.x=i*LARGEUR_TILE;
            VecteurPosition.y=j*HAUTEUR_TILE;
            if(map[i][j]==CHEMIN){
                 SDL_RenderCopy(rendu, texture3, NULL, &VecteurPosition); /* On charge la bonne texture en fonction du statut de la case de la matrice*/
            }
            else if(map[i][j]==MUR){
                SDL_RenderCopy(rendu, texture, NULL, &VecteurPosition); /* idem*/
            }
            else if(map[i][j]==ROCHER){
                SDL_RenderCopy(rendu, texture2, NULL, &VecteurPosition);/* idem*/
            }
            else if(map[i][j]==BOMBE){
                SDL_RenderCopy(rendu,texture4,NULL,&VecteurPosition);/* idem*/
            }
            else if(map[i][j]==BOMBE_EXPLOSION){
                SDL_RenderCopy(rendu,texture5,NULL,&VecteurPosition);/* idem*/
            }
            
            else if(map[0][0]==WIN){   /* Cas un peu spécial lorsqu'on perd/gagne */
                VecteurPosition.x=0; /* On modifie le Vecteur position pour qu'il se place à l'origine */
                VecteurPosition.y=0;
                VecteurPosition.h=13*HAUTEUR_TILE; /* On joue ensuite sur la hauteur et la largeur pour que l'image prennent toute la fenetre*/
                VecteurPosition.w=13*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, Win, NULL, &VecteurPosition);
            }
            else if(map[0][0]==LOOSE){/* Cas un peu spécial lorsqu'on perd/gagne */
                VecteurPosition.x=0;
                VecteurPosition.y=0;
                VecteurPosition.h=13*HAUTEUR_TILE;
                VecteurPosition.w=13*HAUTEUR_TILE;
                SDL_RenderCopy(rendu, Loose, NULL, &VecteurPosition);
            }
            // else if (map[i][j]==POW){
            //     SDL_RenderCopy(rendu, texture6, NULL, &VecteurPosition);
            // }

        }
    }
    /* On oublie pas de placer également les personnages sur la carte en fonction de leurs propres vecteurPositions*/
    SDL_RenderCopy(rendu, Personnage2, NULL, &VecteurPositionPersonnage2);
    SDL_RenderCopy(rendu, Personnage1, NULL, &VecteurPositionPersonnage);
    //printf("Rendu en cours \n");
    /* On Affiche ensuite la modification*/
    //printf("Valeur qui bug dans le refresh ecran quand la game se termine : %d \n",Carte[10][11]);
    SDL_RenderPresent(rendu);
    //Sleep(2000);
    //SDL_RenderClear(rendu);
    //printf("Rendu fini \n");
}

/* Fonction AfficheEndGame
 
    Paramètres: 
                

    But de la fonction : C'est une fonction test pour afficher GameOver ou YouWin à la fin de la partie
 
 
 */
void AfficheEndGame(){
    SDL_RenderClear(renderer);
     SDL_Surface* AffichageWin=IMG_Load("img/youwin.png");
    SDL_Texture* Win=NULL;

    SDL_Surface* AffichageLoose=IMG_Load("img/gameover.jpg");
    SDL_Texture* Loose=NULL;

     Win=SDL_CreateTextureFromSurface(renderer,AffichageWin);
    Loose=SDL_CreateTextureFromSurface(renderer,AffichageLoose);

    SDL_Rect VecteurPosition;
    if(Carte[0][0]==WIN){
                VecteurPosition.x=0;
                VecteurPosition.y=0;
                VecteurPosition.h=25*HAUTEUR_TILE;
                VecteurPosition.w=25*HAUTEUR_TILE;
                 SDL_RenderCopy(renderer, Win, NULL, &VecteurPosition);
            }
            else if(Carte[0][0]==LOOSE){
                VecteurPosition.x=0;
                VecteurPosition.y=0;
                VecteurPosition.h=100*HAUTEUR_TILE;
                VecteurPosition.w=100*HAUTEUR_TILE;
                SDL_RenderCopy(renderer, Loose, NULL, &VecteurPosition);
            }
        SDL_RenderPresent(renderer);
}
//--------------------------------------------------------------------------------------------------------------

/* Fonction ExplosionInGame
 
    Paramètres: 
                -int coX -> c'est la coordonée en X depuis laquelle la fonction a été appelé 
                -int coY -> c'est la coordonée en X depuis laquelle la fonction a été appelé

    But de la fonction : En prenant en compte les coordonnées passées en paramètres, la fonction test les 4
                         possibilitées et modifie la carte en BOMBE_EXPLOSION , c'est à dire le cas où la bombe est en train d'exploser 
                         On rafraichit ensuite la fenetre pour faire apparaitre la modification
 
 
 */

void ExplosionInGame(int CoX,int CoY){
    //printf("La bombe va explose");
    // SDL_Surface* AffichageBomb=IMG_Load("img/Bamboo-Bloc.png");
    // SDL_Texture* texture=NULL;
    
   
    //     texture=SDL_CreateTextureFromSurface(rendu,AffichageBomb);
    //     SDL_RenderCopy(rendu, texture, NULL, &VecteurPositionPersonnage);
        
        /* Ici, on test les 4 cas possibles
           On fait bien attention de tester seulement si les TILES autour de la bombe sont des rochers
           ou des chemins pour modifier leurs statuts car on ne veut pas modifier par exemple le statut d'un mur indestructible
        */

        int V1=Carte[CoX][CoY-1];
        if(V1==ROCHER || V1==CHEMIN){
            Carte[CoX][CoY-1]=BOMBE_EXPLOSION;
            //return -1;
        }
        V1=Carte[CoX-1][CoY];
        if(V1==ROCHER || V1==CHEMIN){
           Carte[CoX-1][CoY]=BOMBE_EXPLOSION;
            //return -1;
        }
        V1=Carte[CoX+1][CoY];
        if(V1==ROCHER || V1==CHEMIN){
            Carte[CoX+1][CoY]=BOMBE_EXPLOSION;
            //return -1;
        }
         V1=Carte[CoX][CoY+1];
        if(V1==ROCHER || V1==CHEMIN){
           Carte[CoX][CoY+1]=BOMBE_EXPLOSION;
            //return -1;
        }
        //RefreshEcran(Carte,renderer);Carte[11][10]
        printf("Dans l'explosion in game (rendu?) \n");

        /* Actualisation de l'affichage en fonction de la modification de la carte*/
        SDL_Renderer* renderer2=NULL;
        printf("Appel en cours de RefreshEcran avec le renderer2 \n");
       // RefreshEcran(Carte,renderer2);
       // SDL_RenderPresent(renderer);
        SDL_Delay(500);
        
   
        //compteur_bombe++;
        
    
    //Sleep(1000);
    //return compteur_bombe;
   
    
    //SDL_Delay(1000);
    //printf("Bombe Fini");
}

 /* Fonction ConditionVictoire
 
    Paramètres: -int Carte[13][13] -> La matrice qui représente la carte de jeu
                -int appelPerso -> permet d'identifier qui appel la fonction (le joueur 1 ou le joueur 2), utile pour savoir qui a gagner/perdu
    la partie
                -int coX -> c'est la coordonée en X depuis laquelle la fonction a été appelé 
                -int coY -> c'est la coordonée en X depuis laquelle la fonction a été appelé

    But de la fonction : Identifie si un joueur est présent sur une case explosion lors de l'appel de la fonction apres les Sleeps
                         Modifie ensuite la carte de jeu en fonction du statut du JOUEUR 1 (WIN/LOOSE)
                         On Refresh alors l'écran pour faire apparaitre la modification
 
 
 */

void ConditionVictoire(int Carte[13][13],int appelPerso,int coX,int coY){
    
    
    //printf("L'appel a ete fait aux coordonnes [%d][%d] \n",coX,coY);

    /* Ici on va récuperer les coordonnées actualisées des 2 joueurs (car le déplacement est posssible pendant le sleep)*/
    int CoX_Perso1=VecteurPositionPersonnage.x/50;
    int CoY_Perso1=VecteurPositionPersonnage.y/50;
    int CoX_Perso2=VecteurPositionPersonnage2.x/50;
    int CoY_Perso2=VecteurPositionPersonnage2.y/50;
    //printf("Le joueur 1 est aux cordonnees [%d][%d] \n",CoX_Perso1,CoY_Perso1);

    /* On test ensuite les 4 possibilités (la où la bombe explose )*/
    if((CoX_Perso1==coX && CoY_Perso1==coY)|| (CoX_Perso1==coX-1 && CoY_Perso1==coY) || (CoX_Perso1==coX+1 && CoY_Perso1==coY) || (CoX_Perso1==coX && CoY_Perso1==coY-1)
    || (CoX_Perso1==coX && CoY_Perso1==coY+1)){
        if(appelPerso==1){
            printf("Le joueur 1 est mort par suicide \n");
            for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    //printf("Valeur Lecture map apres chargement %d \n",Carte[i][j]);
                                    /* Modification de la carte pour actualiser l'affichage*/
                                    Carte[i][j]=LOOSE;
                                    //printf("Valeur carte %d \n",Carte[i][j]);
                                }
                            }
                            Sleep(250);
                            // Carte[11][10]=LOOSE;
                            // Carte[10][11]=LOOSE;
            //AfficheEndGame();
            // SDL_Surface* AffichageWin=IMG_Load("img/youwin.png");
            //  SDL_Texture* Win=NULL;

            // SDL_Surface* AffichageLoose=IMG_Load("img/gameover.jpg");
            // SDL_Texture* Loose=NULL;
            // Win=SDL_CreateTextureFromSurface(rendu,AffichageWin);
            // Loose=SDL_CreateTextureFromSurface(rendu,AffichageLoose);
           // RefreshEcran(Carte,renderer);
           // Sleep(100);
            printf("valeur qui bug : %d",Carte[10][11]);
            /* Actualisation de l'affichage en fonction de la modification de la carte*/
             //SDL_Delay(2000);
            //SDL_RenderClear(renderer);
            //renderer=NULL;
            printf("Appel du refresh Ecran quand le perso1 se suicide \n");
           // RefreshEcran(Carte,renderer);
           
        }
        else{
            printf("Le joueur 1 est mort par la bombe du joueur 2 \n");
            for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    //printf("Valeur Lecture map apres chargement %d \n",Carte[i][j]);
                                    /* Modification de la carte pour actualiser l'affichage*/
                                    Carte[i][j]=LOOSE;
                                    //printf("Valeur carte %d \n",Carte[i][j]);
                                }
                            }
            

            /* Actualisation de l'affichage en fonction de la modification de la carte*/
           // RefreshEcran(Carte,renderer);
           // SDL_RenderClear(renderer);
        }
        
    }

    if((CoX_Perso2==coX && CoY_Perso2==coY)|| (CoX_Perso2==coX-1 && CoY_Perso2==coY) || (CoX_Perso2==coX+1 && CoY_Perso2==coY) || (CoX_Perso2==coX && CoY_Perso2==coY-1)
    || (CoX_Perso2==coX && CoY_Perso2==coY+1)){
        if(appelPerso==2){
            printf("Le joueur 2 est mort par suicide \n");
            //SDL_RenderClear(renderer);
            //Sleep(1000);
            // SDL_RenderCopy(renderer,Win,NULL,&VecteurPosition);
            //  SDL_RenderPresent(renderer);
            // SDL_RenderCopy(renderer,TexturePourEcriture,NULL,&PositionEcriture);
            // SDL_RenderPresent(renderer);
            // while(appelPerso){
            //     printf("Boucle infini \n");
            // }
             for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    //printf("Valeur Lecture map apres chargement %d \n",Carte[i][j]);
                                    /* Modification de la carte pour actualiser l'affichage*/
                                    Carte[i][j]=WIN;
                                    //printf("Valeur carte %d \n",Carte[i][j]);
                                }
                            }
                           // Sleep(250);
                            // 
            /* Actualisation de l'affichage en fonction de la modification de la carte*/
            printf("Appel du refreshEcran quand le perso1 se suicide \n");
          //  RefreshEcran(Carte,renderer);
           // SDL_RenderClear(renderer);
           // SDL_RenderClear(renderer);
        }
        else{
            printf("Le joueur 2 est mort par la bombe du joueur 1 \n");
           for(int i=0;i<13;i++){
                                for(int j=0;j<13;j++){
                                    //printf("Valeur Lecture map apres chargement %d \n",Carte[i][j]);
                                    /* Modification de la carte pour actualiser l'affichage*/
                                    Carte[i][j]=WIN;
                                    //printf("Valeur carte %d \n",Carte[i][j]);
                                }
                            }
                            //Sleep(250);
                            // Carte[11][10]=WIN;
                            // Carte[10][11]=WIN;
            /* Actualisation de l'affichage en fonction de la modification de la carte*/
            //renderer=NULL;
           // RefreshEcran(Carte,renderer);
           // SDL_RenderClear(renderer);
        }
        
    }
}

// Bug à regler:
    //- Refresh Ecran après l'explosion de la bombe ne marche pas (crash après l'affichage) -> celui la j'ai pas compris , vraiment
    //- Sauvegarde également la position des joueurs dans la sauvegarde pour ne pas avoir de posisiton aléatoires après le chargement (OUBLIE DE MA PART)
    //- Si les joueurs utilisent leurs bombes en simultané alors 1 des 2 bombes ne disparait pas (Encore un prblm de syncro , c'est vraiment la galère
    // avec les threads)

   

   


   

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    

     //- 2 cases bizarres quand on win/loose (je pense que c'est un prblm de syncro avec la fin du threat , il faudrait jeter un oeil à la fin
    // des fonctions getions_bombe) (DONE)

     //- Placement des persos quand on rejoue (DONE)

    // - Perso 2 disparait si suicide (Je pense que c'est un oublie de ma part , soit je ne charge pas les textures soir prblm sur le VecteurPoisition) (DONE)



//Features:
    //-PowerUp pour pimenter la partie (DONE)
    //-Implémentation d'une "IA"pour jouer tt seule

    
    //Plusieurs carte possibles
    //-Implémentation d'image lorsqu'on meurt/met en pause  (SEMI-DONE)
    //Implémentation des musiques et bruitages (Exemple: quand une bombe explose)
    //Amélioration du menu
    //Prise en compte de la souris ?
    //FullScreen pour jouer?