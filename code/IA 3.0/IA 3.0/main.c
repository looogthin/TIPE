#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

int abs(int a) {
	return a < 0 ? -a : a;
}

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

///////////////////////////////////////////////////////////////////////////////////////////////////////

//	Pile pour le graph des arêtes

struct Stack {
	int n;	//	Position dans la pile
	int p;	//	Position du pion
	int w;	//	Poids de l'arete
	struct Stack* prev;	//	Element précédent dans la pile
};
typedef struct Stack Stack;

Stack* stack_create() {
	return NULL;
}

Stack* stack_push(Stack* s, int p, int w) {
	Stack* n = malloc(sizeof(Stack)); assert(n != NULL);
	n->n = s == NULL ? 1 : s->n + 1;
	n->p = p;
	n->w = w;
	n->prev = s;
	return n;
}

Stack* stack_pop(Stack* s) {
	if (s == NULL)
		return NULL;
	Stack* n = s->prev;
	free(s);
	return n;
}

void stack_print(Stack* s) {
	while (s != NULL) {
		printf("%d %d, ", s->p, s->w);
		s = s->prev;
	}
}

void stack_free(Stack* s) {
	if (s == NULL)
		return;
	while (s != NULL)
		s = stack_pop(s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

//	Graphe représenté par listes d'adjacences

struct Graph {
	int color;
	int n;	// Taille du graphe
	Stack** tab;	//	Tableau contenant les piles des arêtes
};
typedef struct Graph Graph;

Graph* graph_create(int color, int n) {
	Graph* g = malloc(sizeof(Graph)); assert(g != NULL);
	g->color = color;
	g->n = n;
	g->tab = malloc(n * sizeof(Stack*)); assert(g != NULL);
	for (int i = 0; i < n; i++)
		g->tab[i] = stack_create();
	return g;
}

void graph_print(Graph* g) {
	for (int i = 0; i < g->n; i++) {
		stack_print(g->tab[i]);
		printf("\n");
	}
}

void graph_free(Graph* g) {
	if (g == NULL)
		return;
	for (int i = 0; i < g->n; i++)
		stack_free(g->tab[i]);
	free(g);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool are_next_line(Plateau *p, int ci, int b) {	//	Teste si deux cases sont sur deux colonnes côtes à côtes (on suppose qu'elles sont sur la même ligne)
	return abs(ci - get_column(p, b)) <= 1;
}

bool are_next_column(Plateau* p, int li, int b) {	//	Test si deux cases sont sur deux lignes côtes à côtes	(on suppose qu'elles sont sur la même colonne)
	return abs(li - get_line(p, b)) <= 1;
}

/*
* Stack* get_neighbours
* 
* Parametres :
*	- Plateau *p : plateau
*	- Graph *g : graph du joueur
*	- int i : position du pion
*	- bool (*f)(Plateau*, int, int) : fonction qui renvoie un booléen
* Return (Stack*) : pile contenant tous les voisins
* 
* Renvoie une pile contenant tous les voisins de cette case, s'ils respectent la condition de la fonction f (en parametre)
*/
Stack* get_neighbours(Plateau* p, Graph* g, int i, bool (*f)(Plateau*, int, int)) {
	Stack* s = NULL;
	int r = i >= p->size ? INT_MIN : i + 1;
	int l = i <= 0 ? INT_MIN : i - 1;
	int t = i < p->n ? INT_MIN : i - p->n;
	int b = i >= p->size - p->n ? INT_MIN : i + p->n;
	int tr = i < p->n ? INT_MIN : i - p->n + 1;
	int bl = i > p->size - p->n - 1 ? INT_MIN : i + p->n - 1;
	int ci = get_column(p,i);	//	Colonne de i
	int li = get_line(p,i);		//	Ligne de i

	if (are_next_line(p, ci, r) && f(p, g->color, r))
		s = stack_push(s, r, 1);
	if (are_next_line(p, ci, l) && f(p, g->color, l))
		s = stack_push(s, l, 1);
	if (are_next_column(p, li, t) && f(p, g->color, t))
		s = stack_push(s, t, 1);
	if (are_next_column(p, li, b) && f(p, g->color, b))
		s = stack_push(s, b, 1);
	if (are_next_line(p, ci, tr) && are_next_column(p, li, tr) && f(p, g->color, tr))
		s = stack_push(s, tr, 1);
	if (are_next_line(p, ci, bl) && are_next_column(p, li, bl) && f(p, g->color, bl))
		s = stack_push(s, bl, 1);
	return s;
}

bool is_color(Plateau* p, int c, int i) {	//	Teste si la case du tableau a la bonne couleur
	return p->tab[i] == c;
}

bool True(Plateau* p, int c, int i) {	//	Uniquement pour avoir le bon type pour un pointeur de fonction
	return true;
}

float kirchoff(Plateau* p, Graph* g, int a) {

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
int evaluate(Plateau *p, Graph *g, Graph *ga) {
	graph_print(g);
	//stack_print(get_neighbours(p, g, 7, True));
	return 0;
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
int ia(Plateau *p, Graph *g, Graph *ga) {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void play(Plateau *p, int a) {	//	Fonction qui place le pion a dans le plateau
	if (p->tab[a] != 0)
		return;
	p->tab[a] = 1;
	print_plateau(p);
	printf("\n");
}

void init_graph(Graph* g, Plateau* p) {	//	Initialise le graph avec toutes les cases en tant que voisin
	for (int i = 0; i < g->n; i++)
		g->tab[i] = get_neighbours(p, g, i, True);
}

int main() {
	Plateau* p = plateau_create(3);
	print_plateau(p);

	Graph* gb = graph_create(1, p->size);	//	Graphe des noirs
	init_graph(gb, p);
	Graph* gw = graph_create(2, p->size);	//	Graphe des blancs
	init_graph(gw, p);
	 
	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a >= 0 && a < 25) {
			play(p, a);
			print_plateau(p);
			printf("\n");
		}
		printf("\neval : %d\n", evaluate(p, gb, gw));
	}

	plateau_free(p);
	graph_free(gb);
	graph_free(gw);
	return 0;
}