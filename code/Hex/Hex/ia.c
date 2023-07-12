#include "ia.h"
#include <stdlib.h>
#include <assert.h>

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
	int* coups = malloc(g->size * sizeof(int));
	if (coups == NULL)
		return -1;
	for (int i = 0; i < g->size; i++)
		coups[i] = INT_MIN;
	int nbCoups = g->isTurnRed ? 500000 : 1;
	int coup = MCTS(g, nbCoups);
	int x = get_column(g, coup);
	int y = get_line(g, coup);
	SDL_Rect rect = { x * g->xPawn + (y * g->xPawn / 2) + g->sizeLeft, y * g->yPawn + g->sizeTop, g->sizePrintPawn, g->sizePrintPawn };
	play(g, x, y, rect);

	Player* player = g->isTurnRed ? g->red : g->blue;
	if (isWinner(g, player))	//	On teste que le joueur qui viens de jouer
		return player->num;
	return 0;
}