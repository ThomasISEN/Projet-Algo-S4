all:
	gcc main.c -o exo -I include -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -LSDL2_image -lSDL2_mixer