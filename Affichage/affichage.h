int menu();
void CreationMap(SDL_Renderer* rendu,int map[13][13]);
void InitialisationJoeur(SDL_Renderer* rendu, int map[13][13]);
void DeplacementPersonnage(SDL_Renderer* rendu,int touche,int Carte[13][13]);
int blocage(int map[13][13],int touche);
void RefreshEcran(int map[13][13],SDL_Renderer* rendu );
void ExplosionInGame(int CoX,int CoY);

void creation_bombe(int compB,int map[13][13],SDL_Renderer* rendu);
void* gestion_bombe(void* arg);
void SauvegardeGame(int carte[13][13]);
void ChargementGame(int carte[13][13]);
void InitialisationJoeur2(SDL_Renderer* rendu, int map[13][13]);
void DeplacementPersonnage2(SDL_Renderer* rendu,int touche,int Carte[13][13]);
int blocage2(int map[13][13],int touche);
void creation_bombe2(int compB,int map[13][13],SDL_Renderer* rendu);
void* gestion_bombe2(void* arg);
void ConditionVictoire(int Carte[13][13],int appelPerso,int coX,int coY);
void AfficheEndGame();
