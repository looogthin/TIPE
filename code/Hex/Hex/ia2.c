#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int min_tab(Game*g, int* tab, int size, int (*f)(Game*, int)) {
	int min = f(g, tab[0]);
	for (int i = 0; i < size; i++)
		if (f(g, tab[i]) < min)
			min = f(g, tab[i]);
	return min;
}

int max_tab(Game *g, int* tab, int size, int (*f)(Game*, int)) {
	int min = f(g, tab[0]);
	for (int i = 0; i < size; i++)
		if (f(g, tab[i]) < min)
			min = f(g, tab[i]);
	return min;
}

double evaluation(Game *g, Player *player) {
	Player* other = g->isTurnRed ? g->blue : g->red;

	//	taille des groups
	double sizeGroups = 0;
	for (int i = 0; i < player->nbGroups; i++)
		sizeGroups += player->groups[i].size * player->groups[i].size;
	for (int i = 0; i < other->nbGroups; i++)
		sizeGroups -= other->groups[i].size * other->groups[i].size;
	sizeGroups *= 2;

	//	Proximite avec les bords
	double prox = 0;
	int(*function)(Game*, int) = g->isTurnRed ? &get_line : &get_column;
	for (int i = 0; i < player->nbGroups; i++) {
		int min = min_tab(g, player->groups[i].tab, player->groups[i].size, function);
		int max = g->size - max_tab(g, player->groups[i].tab, player->groups[i].size, function);
		prox -= min + max;
	}
	function = g->isTurnRed ? &get_column : &get_line;
	for (int i = 0; i < other->nbGroups; i++) {
		int min = min_tab(g, other->groups[i].tab, other->groups[i].size, function);
		int max = g->size - max_tab(g, other->groups[i].tab, other->groups[i].size, function);
		prox += min + max;
	}
	prox *= 0.5;

	return sizeGroups + prox;
}

Stack* get_possible_moves(Game* g) {
	Stack* s = NULL;
	for (int i = 0; i < g->size; i++)
		if (g->plateau[i] == 0)
			s = stack_push(s, i);
	return s;
}

int get_move(Game *g, Player *player, int deep) {
	Stack* s = get_possible_moves(g);
	if (s == NULL)
		return -1;
	double max = -INFINITY;
	int coup = s->val;
	for (; s != NULL; s = stack_pop(s)) {
		g->plateau[s->val] = player->num;
		int numGrp = get_groups(g, player, s->val);
		double val = evaluation(g, player);
		if (val > max) {
			max = val;
			coup = s->val;
		}
		group_pop(player, s->val, numGrp);
		g->plateau[s->val] = 0;
	}
	return coup;
}

int ia_play(Game* g) {
	time_t begin, end;
	time(&begin);

	Player* p = g->isTurnRed ? g->red : g->blue;

	int coup = get_move(g, p, 5);
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
}