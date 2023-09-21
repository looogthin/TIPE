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

const int nbCases = 5;
const int size = 25;
char* plateau;
Player player1 = { '?', NULL, 0 };
Player player2 = { '!', NULL, 0 };
Player* player;
int coup = 0;

void print_groups() {
	printf("nombre de groupes : %d\n", player->nbGroups);
	for (int i = 0; i < player->nbGroups; i++) {
		for (int j = 0; j < player->groups[i].size; j++)
			printf("%c, ", player->groups[i].tab[j] + 'a');
		printf("\n");
	}
}

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

int find_group(int a) {
	for (int i = 0; i < player->nbGroups; i++)
		for (int j = 0; j < player->groups[i].size; j++)
			if (player->groups[i].tab[j] == a)
				return i;
	return -1;
}

bool isInGroup(int g, int a) {
	for (int i = 0; i < player->groups[g].size; i++)
		if (player->groups[g].tab[i] == a)
			return true;
	return false;
}

void swap_groups(int a, int b) {
	Group_Pawns tmp = player->groups[a];
	player->groups[a] = player->groups[b];
	player->groups[b] = tmp;
}

void mergeGroups(int a, int b) {
	int size = player->groups[a].size + player->groups[b].size;
	for (int i = player->groups[a].size, j = 0; i < size; i++, j++)
		player->groups[a].tab[i] = player->groups[b].tab[j];
	player->groups[a].size = size;
	int lastGroup = player->nbGroups - 1;
	swap_groups(b, lastGroup);
	free(player->groups[lastGroup].tab);
	player->nbGroups--;
}

Stack* get_neighbours(int a) {
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

int get_groups(int a) {
	Stack* s = get_neighbours(a);
	int numGrp = -1;

	//	Nouveau groupe (pion tout seul)
	if (s == NULL) {
		player->nbGroups++;
		Group_Pawns g = { malloc(size * sizeof(int)), 1 };
		g.tab[0] = a;
		numGrp = player->nbGroups - 1;
		player->groups[numGrp] = g;
	}

	//	Dans un groupe existant
	else if (s->size == 1) {
		numGrp = find_group(s->val);
		Group_Pawns* g = &player->groups[numGrp];
		g->size++;
		g->tab[g->size - 1] = a;
	}

	//	La c'est la merde
	else if (s->size > 1) {

		//	Premier voisin
		numGrp = find_group(s->val);
		Group_Pawns *grp = &player->groups[numGrp];
		grp->size++;
		grp->tab[grp->size - 1] = a;
		s = stack_pop(s);

		//	Autres voisins
		while (s != NULL) {
			int n = find_group(s->val);
			if (!isInGroup(numGrp, s->val))	//	Le voisin n'est pas dans le groupe
				mergeGroups(numGrp, n);
			s = stack_pop(s);
		}
	}
	if (numGrp < 0)
		printf("erreur lors de l'execution de get_groups\n");
	return numGrp;
}

void pop_groups(int numGrp, int a) {
	if (numGrp < 0 || numGrp > player->nbGroups - 1) {
		printf("Numero de groupe incorrect dans la fonction pop_groups (groupe numero %d alors qu'il y a %d groupes)\n", numGrp, player->nbGroups);
		return;
	}
	Group_Pawns *grp = &(player->groups[numGrp]);
	Stack* s = NULL;
	for (int i = 0; i < grp->size; i++)
		s = stack_push(s, grp->tab[i]);
	grp->size = 0;
	for (; s != NULL; s = stack_pop(s))
		if (s->val != a)
			get_groups(a);
	if (grp->size == 0) {
		swap_groups(numGrp, player->nbGroups - 1);
		player->nbGroups--;
	}
}

int min_tab(int* tab, int size, int (*f)(int)) {
	int min = f(tab[0]);
	for (int i=0; i < size; i++)
		if (f(tab[i]) < min)
			min = f(tab[i]);
	return min;
}

int max_tab(int* tab, int size, int (*f)(int)) {
	int min = f(tab[0]);
	for (int i = 0; i < size; i++)
		if (f(tab[i]) < min)
			min = f(tab[i]);
	return min;
}

double evaluation() {
	Player* other = player == &player1 ? &player2 : &player1;
	
	//	taille des groups
	double sizeGroups = 0;
	for (int i = 0; i < player->nbGroups; i++)
		sizeGroups += player->groups[i].size * player->groups[i].size;
	for (int i = 0; i < other->nbGroups; i++)
		sizeGroups -= other->groups[i].size * other->groups[i].size;
	sizeGroups *= 2;

	//	Proximite avec les bords
	double prox = 0;
	int(*function)(int) = player == &player1 ? &get_line : &get_column;
	for (int i = 0; i < player->nbGroups; i++) {
		int min = min_tab(player->groups[i].tab, player->groups[i].size, function);
		int max = size - max_tab(player->groups[i].tab, player->groups[i].size, function);
		prox += min + max;
	}
	function = player == &player1 ? &get_column : &get_line;
	for (int i = 0; i < other->nbGroups; i++) {
		int min = min_tab(other->groups[i].tab, other->groups[i].size, function);
		int max = size - max_tab(other->groups[i].tab, other->groups[i].size, function);
		prox -= min + max;
	}
	prox *= 1;

	return sizeGroups + prox;
}

double play(int a) {
	if (a < 0 || a >= size)
		return -1;
	plateau[a] = player->c;
	get_groups(a);
	coup++;
	return evaluation();
}

Stack* coups_dispo() {
	Stack* s = NULL;
	for (int i = 0; i < size; i++) {
		if (plateau[i] == 0)
			s = stack_push(s, i);
	}
	return s;
}

double minmax(int deep, bool isIA) {
	if (isIA && deep <= 0)
		return evaluation();

	double max = isIA ? -10000000000.0 : 10000000000.0;
	for (Stack* s = coups_dispo(); s != NULL; s = stack_pop(s)) {
		//printf("minmax\n");
		plateau[s->val] = player2.c;
		int grp = get_groups(s->val);
		double val = minmax(deep - 1, !isIA);
		if (isIA && val > max)
			max = val;
		else if (!isIA && val < max)
			max = val;
		pop_groups(grp, s->val);
		plateau[s->val] = 0;
	}

	return max;
}

int ia(int deep) {
	if (deep <= 0)
		return -1;

	double max = -10000000000;
	int coup = -1;
	for (Stack* s = coups_dispo(); s != NULL; s = stack_pop(s)) {
		//printf("ia\n");
		plateau[s->val] = player2.c;
		int numGrp = get_groups(s->val);
		double val = minmax(deep, true);
		printf("%c : %f \n", s->val + 'a', val);
		if (val > max) {
			max = val;
			coup = s->val;
		}
		pop_groups(numGrp, s->val);
		plateau[s->val] = 0;
	}

	return coup;
}

int main() {
	plateau = malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		plateau[i] = 0;
	print_plateau();
	printf("\n");

	player1.groups = malloc(size * sizeof(Group_Pawns));
	player1.vus = malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++)
		player1.vus[i] = false;
	player2.groups = malloc(size * sizeof(Group_Pawns));
	player2.vus = malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++)
		player2.vus[i] = false;
	player = &player1;
	
	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c >= 'a' && c <= 'y') {
			double val = play(c - 'a');
			print_plateau();
			printf("\n");
			print_groups();
			printf("player : %c\n", player->c);
			printf("evaluation : %f\n", val);
			print_groups();
			printf("\n");
			player = player == &player1 ? &player2 : &player1;

			int coup = ia(2);
			val = play(coup);
			print_plateau();
			printf("\n");
			printf("coup : %c\n", coup + 'a');
			print_groups();
			printf("player : %c\n", player->c);
			printf("evaluation : %f\n", val);
			printf("\n");
			player = player == &player1 ? &player2 : &player1;
		}
	}

	free(plateau);
	return 0;
}