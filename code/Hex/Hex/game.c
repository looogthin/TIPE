#include "game.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

int get_line(Game *g, int a) {
	return a / g->nbCases;
}

int get_column(Game *g, int a) {
	return a % g->nbCases;
}

Player* initPlayer(Game *game, Player* p, int num) {
	p = malloc(sizeof(Player));
	if (p == NULL)
		return NULL;
	p->nb = 0;
	p->pawn = malloc(game->nbCases * game->nbCases * sizeof(int));
	p->rects = malloc(game->nbCases * game->nbCases * sizeof(SDL_Rect) / 2);
	p->num = num;
	return p;
}

void addPawn(Game *g, Player* p, int x, int y, SDL_Rect rect) {
	p->pawn[p->nb * 2] = x;
	p->pawn[p->nb * 2 + 1] = y;
	p->rects[p->nb] = rect;
	p->nb++;
	g->plateau[y * g->nbCases + x] = p->num;
}

bool areInContact(int x1, int y1, int x2, int y2) {
	if ((x1 == x2 && y1 - 1 == y2) ||
		(x1 == x2 && y1 + 1 == y2) ||
		(x1 - 1 == x2 && y1 == y2) ||
		(x1 + 1 == x2 && y1 == y2) ||
		(x1 + 1 == x2 && y1 - 1 == y2) ||
		(x1 - 1 == x2 && y1 + 1 == y2))
		return true;
	return false;
}

Stack* get_neighbours(Game* g, bool* vus, int player, int a) {
	Stack* s = NULL;
	int max = g->nbCases - 1;
	int l = a - 1;
	if (!vus[l] && get_column(g, a) > 0 && g->plateau[l] == player)
		s = stack_push(s, l);
	int r = a + 1;
	if (!vus[r] && get_column(g, a) < max && g->plateau[r] == player)
		s = stack_push(s, r);
	int t = a - g->nbCases;
	if (!vus[t] && get_line(g, a) > 0 && g->plateau[t] == player)
		s = stack_push(s, t);
	int b = a + g->nbCases;
	if (!vus[b] && get_line(g, a) < max && g->plateau[b] == player)
		s = stack_push(s, b);
	int tr = a - g->nbCases + 1;
	if (!vus[tr] && get_column(g, a) < max && get_line(g, a) > 0 && g->plateau[tr] == player)
		s = stack_push(s, tr);
	int bl = a + g->nbCases - 1;
	if (!vus[bl] && get_column(g, a) > 0 && get_line(g, a) < max && g->plateau[bl] == player)
		s = stack_push(s, bl);
	return s;
}

bool isNextWinner(Game* g, bool* vus, int player, int a) {
	if (g->plateau[a] != player)
		return false;
	if (player == 1 && get_line(g, a) == g->nbCases - 1)
		return true;
	if (player == 2 && get_column(g, a) == g->nbCases - 1)
		return true;
	vus[a] = true;
	Stack* s = get_neighbours(g, vus, player, a);
	while (s != NULL) {
		if (isNextWinner(g, vus, player, s->val)) {
			stack_delete(s);
			return true;
		}
		s = stack_pop(s);
	}
	return false;
}

bool isWinner(Game* g, Player *player) {
	bool* vus = malloc(g->nbCases * g->nbCases * sizeof(bool));
	if (vus == NULL)
		return false;
	for (int i = 0; i < g->nbCases * g->nbCases; i++)
		vus[i] = false;
	Stack* s = NULL;
	if (player->num == 1) {
		for (int i = 0; i < g->nbCases; i++)
			if (g->plateau[i] == player->num)
				s = stack_push(s, i);
	}
	else if (player->num == 2) {
		for (int i = 0; i < g->nbCases; i++)
			if (g->plateau[i * g->nbCases] == player->num)
				s = stack_push(s, i * g->nbCases);
	}

	while (s != NULL) {
		vus[s->val] = true;
		if (isNextWinner(g, vus, player->num, s->val)) {
			stack_delete(s);
			free(vus);
			return true;
		}
		s = stack_pop(s);
	}

	return false;
}

int play(Game *g, int x, int y, SDL_Rect rect) {
	Player* p = g->isTurnRed ? g->red : g->blue;
	if (g->plateau[y * g->nbCases + x] == 0)
		addPawn(g, p, x, y, rect);
	else
		return -1;
	if (isWinner(g, p))
		return p->num;
	return 0;
}