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
    
int main(int argc, char *argv[]){
   srand(time(NULL));
   menu();
   return 0;
}