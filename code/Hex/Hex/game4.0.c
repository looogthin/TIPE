#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int get_line(Game* g, int a) {
	return a / g->nbCases;
}

int get_column(Game* g, int a) {
	return a % g->nbCases;
}

Player* initPlayer(Game* game, Player* p, int num) {
	p = malloc(sizeof(Player));
	if (p == NULL)
		return NULL;
	p->nb = 0;
	p->pawn = malloc(game->size * sizeof(int));
	p->rects = malloc(game->size * sizeof(SDL_Rect) / 2);
	p->num = num;
	p->groups = malloc(game->size * sizeof(Group_Pawns));
	p->nbGroups = 0;
	p->vus = malloc(game->size * sizeof(bool));
	return p;
}

void addPawn(Game* g, Player* p, int x, int y, SDL_Rect rect) {
	p->pawn[p->nb * 2] = x;
	p->pawn[p->nb * 2 + 1] = y;
	p->rects[p->nb] = rect;
	p->nb++;
	g->plateau[y * g->nbCases + x] = p->num;
	g->nbCoups++;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Stack* get_neighbours(Game* g, int player, int a) {
	Stack* s = NULL;
	int max = g->nbCases - 1;
	int l = a - 1;
	if (get_column(g, a) > 0 && g->plateau[l] == player)
		s = stack_push(s, l);
	int r = a + 1;
	if (get_column(g, a) < max && g->plateau[r] == player)
		s = stack_push(s, r);
	int t = a - g->nbCases;
	if (get_line(g, a) > 0 && g->plateau[t] == player)
		s = stack_push(s, t);
	int b = a + g->nbCases;
	if (get_line(g, a) < max && g->plateau[b] == player)
		s = stack_push(s, b);
	int tr = a - g->nbCases + 1;
	if (get_column(g, a) < max && get_line(g, a) > 0 && g->plateau[tr] == player)
		s = stack_push(s, tr);
	int bl = a + g->nbCases - 1;
	if (get_column(g, a) > 0 && get_line(g, a) < max && g->plateau[bl] == player)
		s = stack_push(s, bl);
	return s;
}

int incr_for_isWinner(Game* g, int player, int i) {
	return player == 1 ? i + 1 : i + g->nbCases;
}

bool existsFalse(bool* tab, int n) {
	for (int i = 0; i < n; i++)
		if (!tab[i])
			return true;
	return false;
}

bool isWinner_rec(Game* g, int player, int a, bool* vus) {
	if (vus[a])
		return false;
	vus[a] = true;
	for (Stack* s = get_neighbours(g, player, a); s != NULL; s = stack_pop(s)) {
		if ((player == 1 && get_line(g, s->val) == g->nbCases - 1) ||
			(player == 2 && get_column(g, s->val) == g->nbCases - 1))
			return true;
		if (isWinner_rec(g, player, s->val, vus)) {
			return true;
		}
	}
	return false;
}

bool isWinner(Game* g, int player) {
	bool win = false;
	bool* vus = malloc(g->size * sizeof(bool)); assert(vus != NULL);
	for (int i = 0; i < g->size; i++)
		vus[i] = false;

	for (int i = 0; existsFalse(vus, g->size) && ((player == 1 && i < g->nbCases) || (player == 2 && i < g->size)); i = incr_for_isWinner(g, player, i)) {
		if (g->plateau[i] != player)
			continue;
		if (isWinner_rec(g, player, i, vus)) {
			win = true;
			break;
		}
	}

	free(vus);
	return win;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

int play(Game* g, int x, int y, SDL_Rect rect) {
	Player* p = g->isTurnRed ? g->red : g->blue;
	if (g->plateau[y * g->nbCases + x] == 0)
		addPawn(g, p, x, y, rect);
	else
		return -1;
	if (isWinner(g, p->num))	//	On teste que le joueur qui viens de jouer
		return p->num;
	return 0;
}