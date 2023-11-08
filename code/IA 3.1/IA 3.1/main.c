#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////

//	Plateau représenté par un tableau d'entiers

struct Plateau {
	int n;		//	Nombre de cases sur une ligne
	int size;	//	Nombre total de cases
	int* tab;	//	Tableau contenant les pions
};
typedef struct Plateau Plateau;

Plateau* plateau_create(int n) {
	Plateau* p = malloc(sizeof(Plateau)); assert(p != NULL);
	p->n = n;
	p->size = n * n;
	p->tab = malloc(p->size * sizeof(int));
	for (int i = 0; i < p->size; i++)
		p->tab[i] = 0;
	return p;
}

int get_column(Plateau* p, int a) {
	return a % p->n;
}

int get_line(Plateau* p, int a) {
	return a / p->n;
}

void print_plateau(Plateau* p) {
	for (int i = 0; i < p->n; i++) {
		for (int j = 0; j < i; j++)
			printf(" ");
		for (int j = 0; j < p->n; j++) {
			if (p->tab[i * p->n + j] == 0)
				printf("%c ", i * p->n + j + 'a');
			else
				printf("%d ", p->tab[i * p->n + j]);
		}
		printf("\n");
	}
}

void plateau_free(Plateau* p) {
	if (p == NULL)
		return;
	free(p->tab);
	free(p);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Case {
	int pos;//	Position du pion
	int r;	//	Resistance droite
	int b;	//	Resistance bas
	int br;	//	Resistance bas-droite
};
typedef struct Case Case;

void print_case(Case c) {
	printf("%c : %d, %d, %d", c.pos + 'a', c.r, c.b, c.br);
}

//	Graphe représenté par listes d'adjacences

struct Graph {
	int color;
	int n;	// Taille du graphe
	Case* tab;	//	Tableau contenant les piles des arêtes
};
typedef struct Graph Graph;

Graph* graph_create(int color, int n) {
	Graph* g = malloc(sizeof(Graph)); assert(g != NULL); assert(g != NULL);
	g->color = color;
	g->n = n;
	g->tab = malloc(n * sizeof(Case)); assert(g != NULL); assert(g->tab != NULL);
	return g;
}

void graph_print(Graph* g) {
	for (int i = 0; i < g->n; i++) {
		print_case(g->tab[i]);
		printf("\n");
	}
}

void graph_free(Graph* g) {
	if (g == NULL)
		return;
	free(g->tab);
	free(g);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

float tension(Plateau* p, Graph* g) {

}

/*
	int evaluate

	Parametres :
		- Plateau *p : plateau
		- Graph *g : graph du joueur actuel
		- Graph *ga : graph du joueur adverse
	Return (int) : valeur de la position du joueur par rapport à son adversaire

	Teste si la position est avantageuse ou non pour le joueur
*/
int evaluate(Plateau* p, Graph* g, Graph* ga) {
	return log(tension(p, g) / tension(p, ga));
}

/*
* void ia
*
* Parametres :
*	- Plateau *p
*	- Graph *g : Graph du joueur actuelle
*	- Graph *ga : Graph du joueur adverse
* Return (int) : Le coup que l'ia va jouer
*
* Determine le meilleur coup possible a jouer pour l'IA
*/
int ia(Plateau* p, Graph* g, Graph* ga) {
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void play(Plateau* p, int a) {	//	Fonction qui place le pion a dans le plateau
	if (p->tab[a] != 0)
		return;
	p->tab[a] = 1;
	print_plateau(p);
	printf("\n");
}

void init_graph(Plateau* p, Graph* g, bool isJ1) {
	int i = 0;
	for (; i < p->size; i++) {
		Case c = { i, 1, 1, 1 };
		if (get_column(p, i) == 0)	//	premiere colonne --> aucune connexion pour le bas
			c.b = INT_MIN;
		else if (get_column(p, i) == p->n - 1)	//	derniere colonne --> victoire pour J2 à droite
			c.r = isJ1 ? INT_MIN : -1;
		if (get_line(p, i) == p->n - 1) {	//	derniere ligne --> aucune connexion pour le bas + victoire pour J1 en bas à droite
			c.b = INT_MIN;
			c.br = isJ1 ? -1 : INT_MIN;
		}
		g->tab[i] = c;
	}
}

int main() {
	Plateau* p = plateau_create(3);
	print_plateau(p);

	Graph* g1 = graph_create(1, p->size);	//	Graphe de J1 (chemin vertical)
	init_graph(p, g1, true);
	Graph* g2 = graph_create(2, p->size);	//	Graphe de J2 (chemin horizontal)
	init_graph(p, g2, false);

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a >= 0 && a < 25) {
			play(p, a);
			printf("\n");
		}
		printf("\neval : %d\n", evaluate(p, g1, g2));
	}

	plateau_free(p);
	graph_free(g1);
	graph_free(g2);
	return 0;
}