#include "src/include/SDL2/SDL.h"
#include "src/include/SDL2/SDL_image.h"
#include "src/include/SDL2/SDL_ttf.h"
#include "src/include/SDL2/SDL_mixer.h"
#include<stdio.h>
#include <stdlib.h>

void menu(SDL_Window *fenetre,SDL_Renderer *renderer, SDL_Surface *image);


int main(int argc, char *argv[]){

    SDL_Window *fenetre=NULL;
    SDL_Renderer *renderer=NULL;

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
    fenetre=SDL_CreateWindow("Projet Bomberman",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,0);
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
    SDL_Surface *EcritureNouvellePartie=TTF_RenderText_Solid(TestNouvellePartie,"Appuie sur 1 pour jouez une partie!",rouge);

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
                            printf("Appuie sur 1");

                            menu(fenetre, renderer, image);
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


void menu(SDL_Window *fenetre,SDL_Renderer *renderer, SDL_Surface *image){
    
    if (SDL_RenderClear(renderer)!=0){
        printf("Problème clear windows");
    }

    


    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(fenetre);
}