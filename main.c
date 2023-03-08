/**
 * @file main.c
 *
 * @brief Programme de lancement du jeu en langage C.
 */
#include "src/include/SDL2/SDL.h"
#include "src/include/SDL2/SDL_image.h"
#include "src/include/SDL2/SDL_ttf.h"
#include "src/include/SDL2/SDL_mixer.h"
#include"Affichage/affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <pthread.h>
#include<string.h>

/**
 * @defgroup affichage Fonctions d'affichage
 * Ce groupe contient des fonctions pour effectuer les affichages de la librairie SDL.
 */
    
int main(int argc, char *argv[]){
   srand(time(NULL));
   menu();
   return 0;
}