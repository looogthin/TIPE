#include "game.h"
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
	p->pawn = malloc(game->size * sizeof(int));
	p->rects = malloc(game->size * sizeof(SDL_Rect) / 2);
	p->num = num;
	p->groups = malloc(game->size * sizeof(Group_Pawns));
	p->nbGroups = 0;
	p->vus = malloc(game->size * sizeof(bool));
	return p;
}

int find_group(Player* player, int a) {
	for (int i = 0; i < player->nbGroups; i++)
		for (int j = 0; j < player->groups[i].size; j++)
			if (player->groups[i].tab[j] == a)
				return i;
	return -1;
}

bool isInGroup(Player* player, int g, int a) {
	for (int i = 0; i < player->groups[g].size; i++)
		if (player->groups[g].tab[i] == a)
			return true;
	return false;
}

void swap_groups(Player* player, int a, int b) {
	Group_Pawns tmp = player->groups[a];
	player->groups[a] = player->groups[b];
	player->groups[b] = tmp;
}

void mergeGroups(Player* player, int a, int b) {
	int size = player->groups[a].size + player->groups[b].size;
	for (int i = player->groups[a].size, j = 0; i < size; i++, j++)
		player->groups[a].tab[i] = player->groups[b].tab[j];
	player->groups[a].size = size;
	int lastGroup = player->nbGroups - 1;
	swap_groups(player, b, lastGroup);
	free(player->groups[lastGroup].tab);
	player->nbGroups--;
}

Stack* get_neighbours_evaluation(Game* g, Player* player, int a) {
	Stack* s = NULL;
	int max = g->nbCases - 1;
	int l = a - 1;
	if (get_column(g, a) > 0 && g->plateau[l] == player->num)
		s = stack_push(s, l);
	int r = a + 1;
	if (get_column(g, a) < max && g->plateau[r] == player->num)
		s = stack_push(s, r);
	int t = a - g->nbCases;
	if (get_line(g, a) > 0 && g->plateau[t] == player->num)
		s = stack_push(s, t);
	int b = a + g->nbCases;
	if (get_line(g, a) < max && g->plateau[b] == player->num)
		s = stack_push(s, b);
	int tr = a - g->nbCases + 1;
	if (get_column(g, a) < max && get_line(g, a) > 0 && g->plateau[tr] == player->num)
		s = stack_push(s, tr);
	int bl = a + g->nbCases - 1;
	if (get_column(g, a) > 0 && get_line(g, a) < max && g->plateau[bl] == player->num)
		s = stack_push(s, bl);
	return s;
}

/*int get_groups(Game* g, Player* player, int a) {
	Stack* s = get_neighbours_evaluation(g, player, a);
	int numGrp = 0;

	//	Nouveau groupe (pion tout seul)
	if (s == NULL) {
		player->nbGroups++;
		Group_Pawns grp = { malloc(g->size * sizeof(int)), 1 };
		grp.tab[0] = a;
		player->groups[player->nbGroups - 1] = grp;
		numGrp = player->nbGroups - 1;
	}

	//	Ajouter a un groupe existant
	else {
		//	Premier voisin
		numGrp = find_group(player, s->val);
		player->groups[numGrp].size++;
		player->groups[numGrp].tab[player->groups[numGrp].size - 1] = a;
		s = stack_pop(s);

		//	Autres voisins
		while (s != NULL) {
			int n = find_group(player, s->val);
			if (!isInGroup(player, numGrp, s->val))	//	Le voisin n'est pas dans le groupe -> il faut fusionner deux groupes
				mergeGroups(player, numGrp, n);
			s = stack_pop(s);
		}
	}

	return numGrp;
}

bool group_pop(Player* player, int a, int numGrp) {
	if (numGrp < 0 || numGrp >= player->nbGroups || a < 0 || a >= player->groups[numGrp].size)
		return false;
	for (int i = 0; i < player->groups[numGrp].size; i++) {
		if (player->groups[numGrp].tab[i] == a) {
			int tmp = player->groups[numGrp].tab[i];
			player->groups[numGrp].tab[i] = player->groups[numGrp].tab[player->groups[numGrp].size - 1];
			player->groups[numGrp].tab[player->groups[numGrp].size - 1] = tmp;
			player->groups[numGrp].size--;
			return true;
		}
	}
	return false;
}*/

int get_groups(Game *g,Player *player, int a) {
	Stack* s = get_neighbours_evaluation(g, player, a);
	int numGrp = 0;

	//	Nouveau groupe (pion tout seul)
	if (s == NULL) {
		player->nbGroups++;
		Group_Pawns grp = { malloc(g->size * sizeof(int)), 1 };
		grp.tab[0] = a;
		player->groups[player->nbGroups - 1] = grp;
	}

	//	Dans un groupe existant
	else if (s->size == 1) {
		Group_Pawns* grp = &player->groups[find_group(player, s->val)];
		grp->size++;
		grp->tab[grp->size - 1] = a;
	}

	//	La c'est la merde
	else if (s->size > 1) {

		//	Premier voisin
		numGrp = find_group(player, s->val);
		player->groups[numGrp].size++;
		player->groups[numGrp].tab[player->groups[numGrp].size - 1] = a;
		s = stack_pop(s);

		//	Autres voisins
		while (s != NULL) {
			int n = find_group(player, s->val);
			if (!isInGroup(player, numGrp, s->val))	//	Le voisin n'est pas dans le groupe
				mergeGroups(player, numGrp, n);
			s = stack_pop(s);
		}
	}

	return numGrp;
}

bool group_pop(Player* player, int a, int g) {
	if (g < 0 || g >= player->nbGroups || a < 0 || a >= player->groups[g].size)
		return false;
	for (int i = 0; i < player->groups[g].size; i++) {
		if (player->groups[g].tab[i] == a) {
			player->groups[g].tab[i] = player->groups[g].tab[player->groups[g].size - 1];
			player->groups[g].size--;
			if (player->groups[g].size == 0) {
				player->groups[g] = player->groups[player->nbGroups - 1];
				player->nbGroups--;
			}
		}
	}
	return true;
}

void addPawn(Game *g, Player* p, int x, int y, SDL_Rect rect) {
	p->pawn[p->nb * 2] = x;
	p->pawn[p->nb * 2 + 1] = y;
	p->rects[p->nb] = rect;
	p->nb++;
	g->plateau[y * g->nbCases + x] = p->num;
	g->nbCoups++;
	get_groups(g, p, y * g->nbCases + x);
}

bool isWinner(Game* g, Player* player) {
	for (int i = 0; i < player->nbGroups; i++) {
		Group_Pawns grp = player->groups[i];
		bool side1 = false;
		bool side2 = false;
		for (int j = 0; j < grp.size; j++) {
			if ((player->num == 1 && get_line(g, grp.tab[j]) == g->nbCases - 1) || (player->num == 2 && get_column(g, grp.tab[j]) == g->nbCases - 1))
				side1 = true;
			if ((player->num == 1 && get_line(g, grp.tab[j]) == 0) || (player->num == 2 && get_column(g, grp.tab[j]) == 0))
				side2 = true;
		}
		if (side1 && side2)
			return true;
	}
	return false;
}

int play(Game *g, int x, int y, SDL_Rect rect) {
	Player* p = g->isTurnRed ? g->red : g->blue;
	if (g->plateau[y * g->nbCases + x] == 0)
		addPawn(g, p, x, y, rect);
	else
		return -1;
	if (isWinner(g, p))	//	On teste que le joueur qui viens de jouer
		return p->num;
	return 0;
}