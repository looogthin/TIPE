#ifndef GAME_H
#define GAME_H

#include "stack.h"
#include <SDL.h>
#include <stdbool.h>

/*
*	Structure d'un joueur
*/
struct Player {
	unsigned int nb;	//	Nombre de pions qu'a pose le joueur
	int* pawn;			//	Tableau contenant les positions des pions (position x aux indices paires et y aux impaires)
	SDL_Rect* rects;	//	Tableau contenant les rectangles des pions
	int num;			//	Numero du joueur (1 s'il doit relier de haut en bas, 2 s'il doit relier de gauche a droite)
};
typedef struct Player Player;

/*
*	Structure qui contient toutes les informations du jeu et les pointeurs vers les 2 joueurs
*/
struct Game {
	int nbCases;		//	Nombre de cases sur la largeur
	int xPawn;			//	taille x d'un pion
	int yPawn;			//	taille y d'un pion
	int sizePrintPawn;	//	taille affiche du pion
	int sizeLeft;		//	Espace a gauche du plateau
	int sizeTop;		//	Espace en haut du plateau
	bool isTurnRed;		//	Vaut true si c'est le tour du joueur rouge, false sinon
	Player* red;		//	Joueur rouge
	Player* blue;		//	Joueur bleu
	int* plateau;		//	Plateau (une case vaut le numero du joueur ou 0 si elle est vide)
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
* bool areInContact
* Parametres :
*	- int x1 : position x de la 1ere case a tester
*	- int y1 : position y de la 1ere case a tester
*	- int x2 : position x de la 2e case a tester
*	- int y2 : position y de la 2e case a tester
* Return bool : true si les 2 cases se touchent, false sinon
*
* Teste si 2 cases se touchent ou non
*/
bool areInContact(int x1, int y1, int x2, int y2);

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
Stack* get_neighbours(Game* g, bool* vus, int player, int a);

/*
* Stack* get_neighbours
* Parametres :
*	- Game *g : parametres du jeu
*	- bool *vus : tableau de booleens qui valent true si le piona deja ete visite
*	- int player : numero du joueur
*	- int a : case a tester
* Return bool : true si le pion et sur un chemin gagnant, false sinon
*
* Renvoie si le pion est sur un chemin gagnant
*/
bool isNextWinner(Game* g, bool* vus, int player, int a);

/*
* bool isWinner
* Parametre :
*	- Game *game
* Return bool : true si le joueur gagne, false sinon
*
* Teste si le joueur gagne
*/
bool isWinner(Game* game, Player* player);

/*
* bool containsPawn
* Parametres :
*	- Player *p : joueur qu'il faut tester
*	- int x : position x a tester
*	- int y : position y a tester
* Return bool : true s'il y a deja un pion sur cette case, false sinon
*
* Teste si la case contient deja un pion
*/
bool containsPawn(Player *p, int x, int y);

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