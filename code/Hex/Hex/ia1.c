#include "ia.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
/*
* 
* 
* --------------------
* ------	1.0	------
* --------------------
* 
* 
*
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
	int nbCoups = g->isTurnRed ? 100000 : 1;

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


/*
* 
* --------------------
* ------	1.5	------
* --------------------
* 
*

void swap(int* tab, int a, int b) {
	int tmp = tab[a];
	tab[a] = tab[b];
	tab[b] = tmp;
}

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

bool toEnd(Game* g, Player *player, int *possibleMoves, int nbPossibleMoves) {
	
	//	Fin de partie aleatoire
	mix(possibleMoves, nbPossibleMoves);
	int middle = nbPossibleMoves % 2 == 0 ? nbPossibleMoves / 2 : nbPossibleMoves / 2 + 1;
	for (int i = 0; i < middle; i++)
		g->plateau[possibleMoves[i]] = player->num;
	player = g->isTurnRed ? g->blue : g->red;
	for (int i = middle; i < nbPossibleMoves; i++)
		g->plateau[possibleMoves[i]] = player->num;

	//	Test victoire
	bool win = true;
	if (isWinner(g, player))
		win = false;			//	a ce moment du programme, la variable player contient l'autre joueur

	//	Enlever les coups
	for (int i = 0; i < nbPossibleMoves; i++)
		g->plateau[possibleMoves[i]] = 0;

	return win;
}

int MCTS(Game* g, Player* player, int nbTests) {
	int nbPossibleMoves = g->size - g->nbCoups;
	int* possibleMoves = malloc(nbPossibleMoves * sizeof(int));
	int* coups = malloc(nbPossibleMoves * sizeof(int));
	int* valMoves = malloc(nbPossibleMoves * sizeof(int));
	Stack* stackMoves = NULL;
	int j = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->plateau[i] == 0) {
			possibleMoves[j] = i;
			coups[j] = i;
			valMoves[j] = 0;
			stackMoves = stack_push(stackMoves, i);
			j++;
		}
	}

	for (int j = 0; j < nbPossibleMoves && stackMoves != NULL; j++, stackMoves = stack_pop(stackMoves)) {
		g->plateau[stackMoves->val] = player->num;	//	mettre le coup

		//	Victoire a 100%
		if (isWinner(g, player)) {
			g->plateau[stackMoves->val] = 0;
			free(possibleMoves);
			free(coups);
			free(valMoves);
			int coup = stackMoves->val;
			stack_delete(stackMoves);
			return coup;
		}

		int index = find(possibleMoves, stackMoves->val, nbPossibleMoves);	//	mettre le coup joue en dernier place, ne pourra pas jouer par dessus
		swap(possibleMoves, index, nbPossibleMoves - 1);
		swap(valMoves, index, nbPossibleMoves - 1);

		for (int i = 0; i < nbTests; i++) {
			if (toEnd(g, player, possibleMoves, nbPossibleMoves - 1))
				valMoves[j]++;
		}

		g->plateau[stackMoves->val] = 0;	//	enlever le coup
	}

	int index = tab_max(valMoves, nbPossibleMoves);
	int coup = coups[index];
	printf("player : %d\n", g->isTurnRed ? 1 : 2);
	printf("coup : %d\n", coup);
	printf("val : %d\n", valMoves[index]);
	free(possibleMoves);
	free(coups);
	free(valMoves);
	return coup;
}

int ia_play(Game* g) {
	time_t begin, end;
	time(&begin);

	const int nbTests = g->isTurnRed ? 500000 : 1;
	Player* p = g->isTurnRed ? g->red : g->blue;

	int coup = MCTS(g, p, nbTests);
	if (coup == -1)
		return -1;
	int x = get_column(g, coup);
	int y = get_line(g, coup);
	SDL_Rect rect = { x * g->xPawn + (y * g->xPawn / 2) + g->sizeLeft, y * g->yPawn + g->sizeTop, g->sizePrintPawn, g->sizePrintPawn };
	play(g, x, y, rect);

	if (isWinner(g, p))
		return p->num;

	time(&end);
	printf("temps : %f\n\n", difftime(end, begin));
	return 0;
}*/