#include "ia.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
/*
int* plateau_copy(Game* g) {
	int* p = malloc(g->size * sizeof(int));
	if (p == NULL)
		return NULL;
	for (int i = 0; i < g->size; i++)
		p[i] = g->plateau[i];
	return p;
}

int next_empty(Game* g, int* plateau, int start) {
	for (int i = start; i < g->size; i++)
		if (plateau[i] == 0)
			return i;
	return -1;
}

void end_game(Game* g, int *plateau, int* coups, int coup) {
	int p = g->isTurnRed ? 1 : 2;
	plateau[coup] = p;
	for (int startNextEmpty = next_empty(g, plateau, 0); startNextEmpty != -1; startNextEmpty = next_empty(g, plateau, startNextEmpty), p = p == 1 ? 2 : 1) {
		int a = rand() % g->size;
		while (plateau[a] != 0)
			a = rand() % g->size;
		plateau[a] = p;
	}
	Player* player = g->isTurnRed ? g->red : g->blue;
	if (isWinner(g, player))
		coups[coup]++;
	else
		coups[coup]--;
}

int max_tableau(int* tab, int size) {
	assert(size > 0);
	int max = 0;
	for (int i = 0; i < size; i++)
		if (tab[i] > tab[max])
			max = i;
	return max;
}

int MCTS(Game* g, int nbTests) {
	int* coups = malloc(g->size * sizeof(int));
	if (coups == NULL)
		return -1;
	for (int i = 0; i < g->size; i++)
		coups[i] = INT_MIN;

	for (int i = 0; i < nbTests; i++) {
		int* plateau = plateau_copy(g);
		int coup = rand() % g->size;
		while (plateau[coup] != 0)
			coup = rand() % g->size;
		end_game(g, plateau, coups, coup);
		free(plateau);
	}
	return max_tableau(coups, g->size);
}

int ia_play(Game *g) {
	time_t begin, end;
	time(&begin);

	int* coups = malloc(g->size * sizeof(int));
	if (coups == NULL)
		return -1;
	for (int i = 0; i < g->size; i++)
		coups[i] = INT_MIN;
	int nbCoups = g->isTurnRed ? 1000000 : 1;

	int coup = MCTS(g, nbCoups);
	int x = get_column(g, coup);
	int y = get_line(g, coup);
	SDL_Rect rect = { x * g->xPawn + (y * g->xPawn / 2) + g->sizeLeft, y * g->yPawn + g->sizeTop, g->sizePrintPawn, g->sizePrintPawn };
	play(g, x, y, rect);

	Player* player = g->isTurnRed ? g->red : g->blue;
	if (isWinner(g, player))	//	On teste que le joueur qui viens de jouer
		return player->num;

	time(&end);
	printf("\n%f\n", difftime(end, begin));
	return 0;
}*/

/* echange les elements a et b dans le tableau */
void swap(int* tab, int a, int b) {
	int tmp = tab[a];
	tab[a] = tab[b];
	tab[b] = tmp;
}

/* melange le tableau */
void mix(int* tab, int size) {
	for (int i = 1; i < size; i++) {
		int j = rand() % i;
		swap(tab, i, j);
	}
}

int tab_max(int* tab, int size) {
	int m = 0;
	for (int i = 0; i < size; i++)
		if (tab[i] > tab[m])
			m = i;
	return m;
}

int find(int* tab, int val, int size) {
	for (int i = 0; i < size; i++)
		if (tab[i] == val)
			return i;
	return -1;
}

/* simule la partie jusqu'a la fin 
* 
* - melange le tableau des coups possibles
* - donne la moitie des cases au premier joueur et la deuxieme moitie a l'autre joueur
*/
/*
bool toEnd(Game* g, Player* player, int* possibleMoves, int nbPossibleMoves) {
	mix(possibleMoves, nbPossibleMoves);
	int middle = nbPossibleMoves / 2;
	if (nbPossibleMoves % 2 != 0)	//	Si il reste un nombre impaire de coups, le joueur actuel a un tour en plus que l'autre
		middle++;
	for (int i = 0; i < middle; i++)
		g->plateau[i] = player->num; 
	player == g->red ? g->blue : g->red;
	for (int i = middle; i < nbPossibleMoves; i++)
		g->plateau[possibleMoves[i]] = player->num;
	
	//	test victoire
	bool win = true;
	if (isWinner(g, player))
		win = false;		//	La variable player contient le numero de l'autre joueur

	//	on enleve les coups
	for (int i = 0; i < nbPossibleMoves; i++)
		g->plateau[possibleMoves[i]] = 0;
	return win;
}

int MCTS(Game* g, const int nbCoups, const int player) {
	Stack* s = NULL;	//	Pile contenant les coups possibles
	int nbPossibleMoves = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->plateau[i] == 0) {
			s = stack_push(s, i);
			nbPossibleMoves++;
		}
	}
	int* coups = malloc(nbPossibleMoves * sizeof(int));	//	tableau contenant les cases des coups possibles
	if (coups == NULL)
		return -1;
	int* val = malloc(nbPossibleMoves * sizeof(int));	//	tableau contenant la valeur de la fonction d'evaluation en fonction du coup
	if (val == NULL) {
		free(coups);
		return -1;
	}
	Stack* tmp = s;
	for (int i = 0; i < nbPossibleMoves && tmp != NULL; i++, tmp = tmp->prev) {
		coups[i] = tmp->val;
		val[i] = 0;
	}

	//	Tester tous les coups
	for (int i = 0; i < nbPossibleMoves; i++, s = stack_pop(s)) {
		g->plateau[s->val] = player; //	jouer
		int index = find(coups, s->val, nbPossibleMoves);	//	On met le coup joue en dernier et il ne sera pas considerer comme possible pour simuler la fin de la partie
		swap(coups, index, nbPossibleMoves - 1);
		swap(val, index, nbPossibleMoves - 1);
		for (int i = 0; i < nbCoups; i++) {
			if (toEnd(g, player == 1 ? g->red : g->blue, coups, nbPossibleMoves - 1))
				val[i]++;
			else
				val[i]--;
		}
		g->plateau[s->val] = 0; //	retour en arriere
	}

	for (int i = 0; i < nbPossibleMoves; i++)
		printf("%d - %d\n", coups[i], val[i]);

	int tabmax = tab_max(val, nbPossibleMoves);
	printf("val :%d\ntab max : %d\nnb coups : %d\n", val[tabmax], tabmax, nbPossibleMoves);
	int coup = coups[tabmax];
	printf("coup : %d\n", coup);
	if (coups != NULL)
		free(coups);
	if (val != NULL)
		free(val);
	return coup;
}*/

bool toEnd(Game* g, int *possibleMoves, int nbPossibleMoves) {
	return false;
}

int MCTS(Game* g, Player* player, int nbCoups) {
	int nbPossibleMoves = g->size - g->nbCoups;
	int* possibleMoves = malloc(nbPossibleMoves * sizeof(int));
	int* coups = malloc(nbPossibleMoves * sizeof(int));
	int* valMoves = malloc(nbPossibleMoves * sizeof(int));
	Stack* stackMoves = NULL;
	for (int i = 0, j = 0; i < g->size; i++) {
		if (g->plateau[i] == 0) {
			possibleMoves[j] = i;
			coups[j] = i;
			valMoves[j] = 0;
			stackMoves = stack_push(stackMoves, i);
			j++;
		}
	}

	for (; stackMoves != NULL; stackMoves = stack_pop(stackMoves)) {
		g->plateau[stackMoves->val] = player->num;	//	mettre le coup
		int index = find(possibleMoves, stackMoves->val, nbPossibleMoves);	//	mettre le coup joue en dernier place, il ne pourra pas etre joue par dessus
		swap(possibleMoves, index, nbPossibleMoves - 1);
		swap(valMoves, index, nbPossibleMoves - 1);

		for (int i = 0; i < nbCoups; i++) {
			if (toEnd(g, possibleMoves, nbPossibleMoves))
				valMoves[i]++;
			else
				valMoves[i]--;
		}

		g->plateau[stackMoves->val] = 0;	//	enlever le coup
	}

	int coup = coups[0];//tab_max(valMoves, nbPossibleMoves)];
	free(possibleMoves);
	free(coups);
	free(valMoves);
	return coup;
}

int ia_play(Game* g) {
	time_t begin, end;
	time(&begin);

	const int nbCoups = g->isTurnRed ? 100 : 1;
	Player* p = g->isTurnRed ? g->red : g->blue;

	int coup = MCTS(g, p, nbCoups);
	if (coup == -1)
		return -1;
	int x = get_column(g, coup);
	int y = get_line(g, coup);
	SDL_Rect rect = { x * g->xPawn + (y * g->xPawn / 2) + g->sizeLeft, y * g->yPawn + g->sizeTop, g->sizePrintPawn, g->sizePrintPawn };
	play(g, x, y, rect);

	if (isWinner(g, p))
		return p->num;

	time(&end);
	printf("\n%f\n", difftime(end, begin));
	return 0;
}