#ifndef GAME_H
#define GAME_H

#include "stack.h"
#include <SDL.h>
#include <stdbool.h>

/*
*	Structure d'une groupe de pions
*/
struct Group_Pawns {
	int* tab;		//	Tableau contenant les pions
	int size;		//	Taille du groupe
};
typedef struct Group_Pawns Group_Pawns;

/*
*	Structure d'un joueur
*/
struct Player {
	unsigned int nb;	//	Nombre de pions qu'a pose le joueur
	int* pawn;			//	Tableau contenant les positions des pions (position x aux indices paires et y aux impaires)
	SDL_Rect* rects;	//	Tableau contenant les rectangles des pions
	int num;			//	Numero du joueur (1 s'il doit relier de haut en bas, 2 s'il doit relier de gauche a droite)
	Group_Pawns* groups;//	Tableau des groupes de pions du joueur
	int nbGroups;		//	Nombre dde groupes de pions du joueur
	bool* vus;			//	Tableau pour savoir si un pion a deja ete ajoute a un groupe de pions
};
typedef struct Player Player;

/*
*	Structure qui contient toutes les informations du jeu et les pointeurs vers les 2 joueurs
*/
struct Game {
	int nbCases;		//	Nombre de cases sur la largeur
	int size;			//	Nombre de cases sur tout le plateau
	int xPawn;			//	taille x d'un pion
	int yPawn;			//	taille y d'un pion
	int sizePrintPawn;	//	taille affiche du pion
	int sizeLeft;		//	Espace a gauche du plateau
	int sizeTop;		//	Espace en haut du plateau
	bool isTurnRed;		//	Vaut true si c'est le tour du joueur rouge, false sinon
	Player* red;		//	Joueur rouge
	Player* blue;		//	Joueur bleu
	int* plateau;		//	Plateau (une case vaut le numero du joueur ou 0 si elle est vide)
	int nbCoups;
};
typedef struct Game Game;

int get_line(Game *g, int a);

int get_column(Game *g, int a);

/*
* bool initPlayer
* Parametre :
*	- Player *p : joueur a initialiser
*	- int nbCases : nombre de cases du plateau
* Return bool : true si l'initialisation a reussis, false sinon
*
* Initialise toutes les variables necessaires au joueur
*/
Player* initPlayer(Game *game, Player *p, int num);

/*
* void addPawn
* Parametres :
*	- Player *p : joueur dans lequel doit etre ajoute le pion
*	- int x : position x du pion
*	- int y : position y du pion
*	- SDL_Rect rect : rectangle du pion a afficher a l'ecran
* Return void
*
* Ajoute un pion au joueur
*/
void addPawn(Game *g, Player* p, int x, int y, SDL_Rect rect);

/*
* Stack* get_neighbours
* Parametres :
*	- Game *g : parametres du jeu
*	- bool *vus : tableau de booleens qui valent true si le piona deja ete visite
*	- int player : numero du joueur
*	- int a : case a tester
* Return Stack *s : pile contenant les voisins non visites de la case
* 
* Renvoie une pile contenant les voisins non visite de la case
*/
Stack* get_neighbours(Game* g, int player, int a);

/*
* bool isWinner
* Parametre :
*	- Game *game
* Return bool : true si le joueur gagne, false sinon
*
* Teste si le joueur gagne
*/
bool isWinner(Game* game, int player);

/*
* int play
* Parametres :
*	- Game *g : parametres du jeu
*	- Player *p : joueur qui a joue
*	- int mouse_x : position x de la souris
*	- int mouse_y : poisition y de la souris
*	- SDL_Rect rect : rectangle du pion a afficher a l'ecran
* Return int :
*	-1 si le pion ne peut pas etre joue a cet emplacement
*	0 si le pion a ete joue et personne ne gagne
*	1 si le joueur rouge gagne
*	2 si le joueur bleu gagne
*
* Joue un tour
*/
int play(Game *game, int x, int y, SDL_Rect rect);

#endif GAME_H