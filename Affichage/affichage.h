int menu();
void CreationMap(SDL_Renderer* rendu,int map[13][13]);
void InitialisationJoeur(SDL_Renderer* rendu, int map[13][13]);
void DeplacementPersonnage(SDL_Renderer* rendu,int touche,int Carte[13][13]);
int blocage(int map[13][13],int touche);
void RefreshEcran(int map[13][13],SDL_Renderer* rendu );