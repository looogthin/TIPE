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
	int size;
	struct Stack* prec;
};
typedef struct Stack Stack;

Stack* stack_push(Stack* s, int val) {
	Stack* n = malloc(sizeof(Stack));
	n->val = val;
	n->prec = s;
	if (s != NULL)
		n->size = s->size + 1;
	else
		n->size = 1;
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
*				GAME
* --------------------------------------------------------
*/

Stack* blackEmptyCells(Plateau* p, int player) {
	Stack* s = NULL;
	for (int i = 0; i < p->size * p->size; i++)
		if (p->tab[i] == player ||p->tab[i] == 0)
			s = stack_push(s, i);
	return s;
}

int* stack_to_tab(Stack* s, int* size) {
	*size = s->size;
	int* tab = malloc(*size * sizeof(int));
	for (int i = 0; i < *size; i++) {
		tab[i] = s->val;
		s = stack_pop(s);
	}
	return tab;
}

void H_search(Plateau *p, int player) {
	Stack* newVC = NULL;
	do {
		int size;
		int* G = stack_to_tab(blackEmptyCells(p, player), &size);
		for (int g = 0; g < size; g++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (i == j)
						continue;
					if (p->tab[g] == player)

				}
			}
		}
	} while (newVC != NULL);
}

void play(Plateau* p, int a, int player) {
	p->tab[a] = player;
	plateau_print(p);
}

int main() {
	Plateau* p = plateau_create(5);
	plateau_print(p);

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a < 0 || a > p->size * p->size)
			continue;
		play(p, a, 1);
	}

	plateau_free(p);
	return 0;
}