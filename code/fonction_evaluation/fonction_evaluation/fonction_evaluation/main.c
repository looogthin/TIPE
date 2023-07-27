#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Stack {
	int val;
	struct Stack* prev;
	int size;
};
typedef struct Stack Stack;

Stack* stack_push(Stack* s, int val) {
	Stack *n = malloc(sizeof(Stack));
	n->val = val;
	n->prev = s;
	n->size = s == NULL ? 1 :  s->size + 1;
	return n;
}

Stack* stack_pop(Stack* s) {
	if (s == NULL)
		return NULL;
	Stack* n = s->prev;
	free(s);
	return n;
}

int* stack_to_tab(Stack* s) {
	if (s == NULL)
		return NULL;
	int stackSize = s->size;
	int* tab = malloc(stackSize * sizeof(int));
	for (int i = 0; i < stackSize; i++, s = stack_pop(s))
		tab[i] = s->val;
	return tab;
}

struct Group_Pawns {
	int* tab;
	int size;
};
typedef struct Group_Pawns Group_Pawns;

struct Player {
	char c;
	Group_Pawns* groups;
	int nbGroups;
	bool* vus;
};
typedef struct Player Player;

void print_groups(Player* p) {
	printf("nombre de groupes : %d\n", p->nbGroups);
	for (int i = 0; i < p->nbGroups; i++) {
		for (int j = 0; j < p->groups[i].size; j++)
			printf("%c, ", p->groups[i].tab[j] + 'a');
		printf("\n");
	}
}

const int nbCases = 5;
const int size = 25;
char* plateau;
Player player1 = { '?', NULL, 0 };
Player player2 = { '!', NULL, 0 };

int get_column(int a) {
	return a % nbCases;
}

int get_line(int a) {
	return a / nbCases;
}

void print_plateau() {
	for (int i = 0; i < nbCases; i++) {
		for (int j = 0; j < nbCases; j++) {
			if (plateau[i * nbCases + j] != 0)
				printf("%c ", plateau[i * nbCases + j]);
			else
				printf("%c ", i * nbCases + j + 'a');
		}
		printf("\n");
	}
}

/*Stack* is_next_in_group(Stack* s, int a, bool* vus, Player* player) {
	if (vus[a])
		return s;
	s = stack_push(s, a);
	vus[a] = true;

	int max = nbCases - 1;
	int l = a - 1;
	if (get_column(a) > 0 && !vus[l] && plateau[l] == player->c)
		s = is_next_in_group(s, l, vus, player);
	int r = a + 1;
	if (get_column(a) < max && !vus[r] && plateau[r] == player->c)
		s = is_next_in_group(s, r, vus, player);
	int t = a - nbCases;
	if (!vus[t] && get_line(a) > 0 && plateau[t] == player->c)
		s = is_next_in_group(s, t, vus, player);
	int b = a + nbCases;
	if (!vus[b] && get_line(a) < max && plateau[b] == player->c)
		s = is_next_in_group(s, b, vus, player);
	int tr = a - nbCases + 1;
	if (!vus[tr] && get_column(a) < max && get_line(a) > 0 && plateau[tr] == player->c)
		s = is_next_in_group(s, tr, vus, player);
	int bl = a + nbCases - 1;
	if (!vus[bl] && get_column(a) > 0 && get_line(a) < max && plateau[bl] == player->c)
		s = is_next_in_group(s, bl, vus, player);
	return s;
}

void get_groups(Player* player) {
	bool* vus = malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++)
		vus[i] = false;

	Group_Pawns g = { NULL, 0 };
	for (int i = 0; i < player->nbGroups; i++)
		player->groups[i] = g;
	player->nbGroups = 0;
	
	for (int i = 0; i < size; i++) {
		if (!vus[i] && plateau[i] == player->c) {
			Stack* s = is_next_in_group(NULL, i, vus, player);
			int groupSize = s == NULL ? 0 : s->size;
			Group_Pawns g = { stack_to_tab(s), groupSize };
			player->nbGroups++;
			player->groups[player->nbGroups - 1] = g;
		}
	}

	free(vus);
}*/

int find_group(int a, Player* player) {
	for (int i = 0; i < player->nbGroups; i++)
		for (int j = 0; j < player->groups[i].size; j++)
			if (player->groups[i].tab[j] == a)
				return i;
	return -1;
}

bool isInGroup(Player *player, int g, int a) {
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

Stack* get_neighbours(Player* player, int a) {
	Stack* s = NULL;
	int max = nbCases - 1;
	int l = a - 1;
	if (get_column(a) > 0 && plateau[l] == player->c)
		s = stack_push(s, l);
	int r = a + 1;
	if (get_column(a) < max && plateau[r] == player->c)
		s = stack_push(s, r);
	int t = a - nbCases;
	if (get_line(a) > 0 && plateau[t] == player->c)
		s = stack_push(s, t);
	int b = a + nbCases;
	if (get_line(a) < max && plateau[b] == player->c)
		s = stack_push(s, b);
	int tr = a - nbCases + 1;
	if (get_column(a) < max && get_line(a) > 0 && plateau[tr] == player->c)
		s = stack_push(s, tr);
	int bl = a + nbCases - 1;
	if (get_column(a) > 0 && get_line(a) < max && plateau[bl] == player->c)
		s = stack_push(s, bl);
	return s;
}

void get_groups(Player *player, int a) {
	Stack* s = get_neighbours(player, a);

	//	Nouveau groupe (pion tout seul)
	if (s == NULL) {
		player->nbGroups++;
		Group_Pawns g = { malloc(size * sizeof(int)), 1 };
		g.tab[0] = a;
		player->groups[player->nbGroups - 1] = g;
	}

	//	Dans un groupe existant
	else if (s->size == 1) {
		Group_Pawns *g = &player->groups[find_group(s->val, player)];
		g->size++;
		g->tab[g->size - 1] = a;
	}

	//	La c'est la merde
	else if (s->size > 1) {

		//	Premier voisin
		int i = find_group(s->val, player);
		player->groups[i].size++;
		player->groups[i].tab[player->groups[i].size - 1] = a;
		s = stack_pop(s);

		//	Autres voisins
		while (s != NULL) {
			int n = find_group(s->val, player);
			if (!isInGroup(player, i, s->val))	//	Le voisin n'est pas dans le groupe
				mergeGroups(player, i, n);
			s = stack_pop(s);
		}
	}
}

void play(int a, Player* player) {
	if (a < 0 || a >= size)
		return;
	plateau[a] = player->c;
	get_groups(player, a);
}

int main() {
	plateau = malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		plateau[i] = 0;
	print_plateau();

	player1.groups = malloc(size * sizeof(Group_Pawns));
	player1.vus = malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++)
		player1.vus[i] = false;
	player2.groups = malloc(size * sizeof(Group_Pawns));
	player2.vus = malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++)
		player2.vus[i] = false;
	
	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		play(c - 'a', &player1);
		print_plateau();
		printf("\n");
		print_groups(&player1);
		printf("\n");
	}

	free(plateau);
	return 0;
}