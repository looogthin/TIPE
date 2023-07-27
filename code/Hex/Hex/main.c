#include "game.h"
#include "ia.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* background = NULL;
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color red = { 255, 45, 0, 255 };
SDL_Color blue = { 0, 12, 255, 255 };

Game game;

/*
* void quit
* Parametres :
*	- SDL_Window *window : fenetre
*	- SDL_Renderer *renderer : renderer
*	- int statut : element a renvoyer (success ou failure)
* Return void
*
* Libere la memoire alloue par la fenetre et le renderer puis ferme le programme
*/
void quit(const char* message1, const char* message2, int statut) {
	printf("%s %s\n", message1, message2);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window != NULL)
		SDL_DestroyWindow(window);
	if (background != NULL)
		SDL_DestroyTexture(background);
	SDL_Quit();
	exit(statut);
}

/*
*	SDL_Texture *loadImage
*	Parametre :
*		- char *nomFichier : nom de l'image a charger
*	Return SDL_Texture * : la texture contenant l'image
*
*	Charge une image et la transforme en texture
*/
SDL_Texture* loadImage(char* nameFile) {
	SDL_Surface* surface = SDL_LoadBMP(nameFile);
	if (surface == NULL)
		quit("Erreur SDL_LoadBMP :", SDL_GetError(), EXIT_FAILURE);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		quit("Erreur SDL_CreateTextureFromSurface :", SDL_GetError(), EXIT_FAILURE);
	SDL_FreeSurface(surface);
	return texture;
}

/*
* void setColor
* Parametres :
*	- SDL_Color c : couleur
* Return void
*
* Change la couleur de dessin du renderer
*/
void setColor(SDL_Color* c) {
	if (SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255) != 0)
		quit("Erreur SDL_SetRenderDrawColor :", SDL_GetError(), EXIT_FAILURE);
}

/*
* void initWindow
* Parametres : void
* Return void
*
* Cree la fenetre et le renderer pour pouvoir dessiner
*/
void initWindow() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0)
		quit("Erreur SDL_Init :", SDL_GetError(), EXIT_FAILURE);

	//	Touver la resolution
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
		quit("Erreur SDL_GetCurrentDisplayMode :", SDL_GetError(), EXIT_FAILURE);

	//	Window
	window = SDL_CreateWindow("Jeu de Hex", 0, 0, mode.w, mode.h, SDL_WINDOW_SHOWN);
	if (window == NULL)
		quit("Erreur SDL_CreateWindow :", SDL_GetError(), EXIT_FAILURE);

	//	Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		quit("Erreur SDL_CreateRenderer :", SDL_GetError(), EXIT_FAILURE);

	//	Background
	background = loadImage("plateau.bmp");
	if (background == NULL)
		quit("Erreur loadImage", "", EXIT_FAILURE);
}

/*
* void initGame
* Parametre : void
* Return void
* 
* Initialise toutes les donnees liees au jeu at aux joueurs
*/
void initGame() {
	game.nbCases = 5;
	game.size = game.nbCases * game.nbCases;
	game.xPawn = 98;
	game.yPawn = 85;
	game.sizePrintPawn = 77;
	game.sizeLeft = 185;
	game.sizeTop = 80;
	game.isTurnRed = true;
	game.red = initPlayer(&game, game.red, 1);
	game.blue = initPlayer(&game, game.blue, 2);
	game.nbCoups = 0;
	game.plateau = malloc(game.nbCases * game.nbCases * sizeof(int));
	if (game.red == NULL || game.blue == NULL || game.plateau == NULL)
		quit("Erreur lors de l'allocation de memoire", "", EXIT_FAILURE);
	for (int i = 0; i < game.nbCases * game.nbCases; i++)
		game.plateau[i] = 0;
}

/*
* bool draw
* Parametre :
*	- SDL_Renderer *renderer : renderer dans lequel il faut dessine
* Return bool :
*	- false si le programme a renctontre une erreur et doit s'arreter
*	- true sinon
*
* Dessine les elements dans le renderer
*/
void draw(SDL_Renderer* renderer) {
	setColor(&black);
	if (SDL_RenderClear(renderer) != 0)
		quit("Erreur SDL_RenderClear :", SDL_GetError(), EXIT_FAILURE);

	//	Background
	SDL_Rect rectBackground = { 0,0,1920,1080 };
	if (SDL_RenderCopy(renderer, background, NULL, &rectBackground) != 0)
		quit("Erreur SDL_RenderCopy :", SDL_GetError(), EXIT_FAILURE);

	//	Pion
	setColor(&red);
	if (SDL_RenderFillRects(renderer, game.red->rects, game.red->nb) != 0)
		quit("Erreur SDL_RenderDrawRects :", SDL_GetError(), EXIT_FAILURE);
	setColor(&blue);
	if (SDL_RenderFillRects(renderer, game.blue->rects, game.blue->nb) != 0)
		quit("Erreur SDL_RenderDrawRects :", SDL_GetError(), EXIT_FAILURE);

	SDL_RenderPresent(renderer);
}

/*
* void mouseButtonLeftPressed
* Parametres :
*	- int mouse_x : position x de la souris
*	- int mouse_y : position y de la souris
* Return void
*/
void mouseButtonLeftPressed(int mouse_x, int mouse_y) {
	int y = (mouse_y - game.sizeTop) / game.yPawn;
	int x = (mouse_x - game.sizeLeft - (y * game.xPawn / 2)) / game.xPawn;
	if (x < 0 || y < 0 || x >= game.nbCases || y >= game.nbCases)
		return;
	Player* p1 = game.isTurnRed ? game.red : game.blue;
	SDL_Rect rect = { x * game.xPawn + (y * game.xPawn / 2) + game.sizeLeft, y * game.yPawn + game.sizeTop, game.sizePrintPawn, game.sizePrintPawn };
	int win = play(&game, x, y, rect);
	if (win == -1)
		return;
	if (win == 0)
		game.isTurnRed = !game.isTurnRed;
	else if (win == 1)
		quit("Red win !", "", EXIT_SUCCESS);
	else if (win == 2)
		quit("Blue win !", "", EXIT_SUCCESS);
	draw(renderer);
	ia_play(&game);
	if (win == 0)
		game.isTurnRed = !game.isTurnRed;
	else if (win == 1)
		quit("Red win !", "", EXIT_SUCCESS);
	else if (win == 2)
		quit("Blue win !", "", EXIT_SUCCESS);
	draw(renderer);
}

/*
* bool event
* Parametre :
* Return bool :
*	- false si le programme a renctontre une erreur et doit s'arreter
*	- true sinon
*
* Gere les evenements de la fenetre
*/
bool event() {
	SDL_Event event;
	SDL_WaitEvent(&event);
	if (event.type == SDL_QUIT)		//	Quitter
		return false;
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {	//	Boutton gauche souris
			mouseButtonLeftPressed(event.button.x, event.button.y);
			return true;
		}
	}
	return true;
}

/*
* int main
*/
int main(int argc, char** argv) {
	//	init
	srand(time(NULL));
	initWindow();
	initGame();

	//	Boucle
	draw(renderer);
	//while (event()) {}
	while (true) {
		int win = ia_play(&game);
		draw(renderer);
		if (win == -1) {
			printf("\n\n\nerror\n\n\n");
			continue;
		}
		else if (win == 0)
			game.isTurnRed = !game.isTurnRed;
		else if (win == 1) {
			printf("Red win !");
			//quit("Red win !", "", EXIT_SUCCESS);
			system("PAUSE");
		}
		else if (win == 2) {
			printf("Blue win !");
			//quit("Blue win !", "", EXIT_SUCCESS);
			system("PAUSE");
		}
	}
		
	// Quitter
	quit("", "", EXIT_SUCCESS);
	return EXIT_FAILURE;
}