#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/*
* --------------------------------------------------------
*				PLATEAU
* --------------------------------------------------------
*/

struct Plateau {
	int size;
	int* tab;
};
typedef struct Plateau Plateau;

Plateau* plateau_create(int size) {
	Plateau* p = malloc(sizeof(Plateau));
	if (p == NULL)
		return NULL;
	p->size = size;
	p->tab = malloc(size * size * sizeof(int));
	if (p->tab == NULL) {
		free(p);
		return NULL;
	}
	for (int i = 0; i < size * size; i++)
		p->tab[i] = 0;
	return p;
}

void plateau_free(Plateau* p) {
	free(p->tab);
	free(p);
}

int get_line(Plateau* p, int a) {
	return a / p->size;
}

int get_column(Plateau* p, int a) {
	return a % p->size;
}

void plateau_print(Plateau* p) {
	for (int i = 0; i < p->size; i++) {
		for (int j = 0; j < p->size; j++) {
			if (p->tab[i * p->size + j] == 0)
				printf("%c ", i * p->size + j + 'a');
			else
				printf("%d ", p->tab[i * p->size + j]);
		}
		printf("\n");
	}
}



/*
* --------------------------------------------------------
*				STACK
* --------------------------------------------------------
*/

struct Stack {
	int val;
	struct Stack* prec;
};
typedef struct Stack Stack;

Stack* stack_push(Stack* s, int val) {
	Stack* n = malloc(sizeof(Stack));
	n->val = val;
	n->prec = s;
	return n;
}

Stack* stack_pop(Stack* s) {
	if (s == NULL)
		return NULL;
	Stack* n = s->prec;
	free(s);
	return n;
}




/*
* --------------------------------------------------------
*				GRAPHE
* --------------------------------------------------------
*/

struct Graphe {	//	representation matrice
	int size;
	double* tab;
};
typedef struct Graphe Graphe;

Graphe* graphe_create(int size) {
	Graphe* g = malloc(sizeof(Graphe));
	g->size = size;
	g->tab = malloc(size * size * sizeof(double));
	for (int i = 0; i < size * size; i++)
		g->tab[i] = INFINITY;
	return g;
}

void graphe_free(Graphe* g) {
	if (g != NULL) {
		if (g->tab != NULL)
			free(g->tab);
		free(g);
	}
}

void graphe_print(Graphe* g) {
	for (int i = 0; i < g->size; i++) {
		for (int j = 0; j < g->size; j++) {
			printf("%f   ", g->tab[i * g->size + j]);
			if (g->tab[i * g->size + j] == INFINITY)
				printf("     ");
		}
		printf("\n");
	}
}




/*
* --------------------------------------------------------
*				GAME
* --------------------------------------------------------
*/

void set_edge(Plateau* p, Graphe* g, int a, int b) {
	if (p->tab[b] == 0 || p->tab[b] == 1) {
		g->tab[a * p->size + b] = 3;
		g->tab[b * p->size + a] = 3;
	}
	else if (p->tab[b] == -1) {
		g->tab[a * p->size + b] = INFINITY;
		g->tab[b * p->size + a] = INFINITY;
	}
}

void set_neighbours(Plateau* p, Graphe *g, int a) {
	if (get_column(p, a) > 0)
		set_edge(p, g, a, a - 1);
	if (get_column(p, a) < p->size)
		set_edge(p, g, a, a + 1);
	else if (get_line(p, a) > 0)
		set_edge(p, g, a, a - p->size);
	else if (get_line(p, a) < p->size - 1)
		set_edge(p, g, a, a + p->size);
	else if (get_line(p, a) > 0 && get_column(p, a) > 0)
		set_edge(p, g, a, a - p->size - 1);
	else if (get_line(p, a) < p->size - 1 && get_column(p, a) < p->size - 1)
		set_edge(p, g, a, a + p->size + 1);
}

void play(Plateau* p, Graphe *g, int a, int player) {
	p->tab[a] = player;
	set_neighbours(p, g, a);
	plateau_print(p);
	graphe_print(g);
}

int main() {
	Plateau* p = plateau_create(5);
	plateau_print(p);

	Graphe* g = graphe_create(p->size);
	for (int i = 0; i < p->size; i++)
		g->tab[i * p->size + i] = 0;

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a < 0 || a > p->size * p->size)
			continue;
		play(p, g, a, 1);
	}

	plateau_free(p);
	return 0;
}