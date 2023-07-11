#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

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

void stack_free(Stack* s) {
	while (s != NULL)
		s = stack_pop(s);
}

void stack_print(Stack* s) {
	while (s != NULL) {
		printf("%d, ", s->val);
		s = stack_pop(s);
	}
}



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
				printf(". ");
			else if(p->tab[i * p->size + j] == 1)
				printf("X ");
			else if (p->tab[i * p->size + j] == 2)
				printf("O ");
		}
		printf("\n");
	}
}





/*
* --------------------------------------------------------
*				GAME
* --------------------------------------------------------
*/

Stack* chemin = NULL;

Stack* get_neighbours(Plateau* p, bool *vus, int player, int i) {
	Stack* s = NULL;
	int max = p->size - 1;
	int l = i - 1;
	if (!vus[l] && get_line(p, i) > 0 && p->tab[l] == player)	//	left
		s = stack_push(s, l);
	int r = i + 1;
	if (!vus[r] && get_line(p, i) < max && p->tab[r] == player)	//	right
		s = stack_push(s, r);
	int t = i - p->size;
	if (!vus[t] && get_column(p, i) > 0 && p->tab[t] == player)	//	top
		s = stack_push(s, t);
	int b = i + p->size;
	if (!vus[b] && get_column(p, i) < max && p->tab[b] == player)	//	bottom
		s = stack_push(s, b);
	int tl = i - p->size - 1;
	if (!vus[tl] && get_line(p, i) > 0 && get_column(p, i) > 0 && p->tab[tl] == player)	//	top left
		s = stack_push(s, tl);
	int br = i + p->size + 1;
	if (!vus[br] && get_line(p, i) < max && get_column(p, i) < max && p->tab[br] == player)	//	bottom right
		s = stack_push(s, br);
	return s;
}

bool isNextWinner(Plateau* p, bool *vus, int player, int i) {
	vus[i] = true;
	if (player == 1 && get_line(p, i) == p->size - 1)
		return true;
	else if (player == 2 && get_column(p, i) == p->size - 1)
		return true;
	chemin = stack_push(chemin, i);
	Stack* s = get_neighbours(p, vus, player, i);
	while (s != NULL) {
		if (isNextWinner(p, vus, player, s->val)) {
			stack_free(s);
			return true;
		}
		s = stack_pop(s);
	}
	chemin = stack_pop(chemin);
	return false;
}

bool isWinner(Plateau* p, int player) {
	bool* vus = malloc(p->size * p->size * sizeof(bool));
	if (vus == NULL)
		exit(EXIT_FAILURE);
	for (int i = 0; i < p->size * p->size; i++)
		vus[i] = false;

	for (int k = 0; k < p->size; k++) {
		chemin = stack_push(chemin, k);
		vus[k] = true;
		Stack* s = get_neighbours(p, vus, player, k);
		while (s != NULL) {
			if (isNextWinner(p, vus, player, s->val)) {
				free(vus);
				stack_free(s);
				return true;
			}
			s = stack_pop(s);
		}
		chemin = stack_pop(chemin);
	}

	free(vus);
	return false;
}

int main() {
	srand(time(NULL));

	Plateau* p = plateau_create(5);

	int player = 1;
	for (int i = 0; i < p->size * p->size - 1;) {
		int a = rand() % (p->size * p->size);
		if (p->tab[a] == 0) {
			p->tab[a] = player;
			if (isWinner(p, player)) {
				char w = player == 1 ? 'X' : 'O';
				printf("\n");
				stack_print(chemin);
				printf("\n");
				printf("%c win !\n\n", w);
				break;
			}
			player = (player == 1) ? 2 : 1;
			i++;
			plateau_print(p);
			printf("\n\n");
			//system("PAUSE");
		}
	}

	plateau_print(p);

	plateau_free(p);
	return 0;
}