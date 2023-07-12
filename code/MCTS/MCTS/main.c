#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <limits.h>

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
	if (p == NULL)
		return;
	if (p->tab != NULL)
		free(p->tab);
	free(p);
}

Plateau* plateau_copy(Plateau *p) {
	Plateau* n = malloc(sizeof(Plateau));
	n->size = p->size;
	n->tab = malloc(n->size * n->size);
	for (int i = 0; i < n->size * n->size; i++)
		n->tab[i] = p->tab[i];
	return n;
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

void tab_bool_print(Plateau* p, bool* tab) {
	printf("\n\n");
	for (int i = 0; i < p->size; i++) {
		for (int j = 0; j < p->size; j++)
			printf("%d ", tab[i * p->size + j]);
		printf("\n");
	}
	printf("\n\n");
}





/*
* --------------------------------------------------------
*				GAME
* --------------------------------------------------------
*/

Stack* get_neighbours(Plateau* p, bool* vus, int player, int a) {
	Stack* s = NULL;
	int max = p->size - 1;
	int l = a - 1;
	if (!vus[l] && get_column(p, a) > 0 && p->tab[l] == player)
		s = stack_push(s, l);
	int r = a + 1;
	if (!vus[r] && get_column(p, a) < max && p->tab[r] == player)
		s = stack_push(s, r);
	int t = a - p->size;
	if (!vus[t] && get_line(p, a) > 0 && p->tab[t] == player)
		s = stack_push(s, t);
	int b = a + p->size;
	if (!vus[b] && get_line(p, a) < max && p->tab[b] == player)
		s = stack_push(s, b);
	int tr = a - p->size + 1;
	if (!vus[tr] && get_column(p, a) < max && get_line(p, a) > 0 && p->tab[tr] == player)
		s = stack_push(s, tr);
	int bl = a + p->size - 1;
	if (!vus[bl] && get_column(p, a) > 0 && get_line(p, a) < max && p->tab[bl] == player)
		s = stack_push(s, bl);
	return s;
}

bool isNextWinner(Plateau* p, bool* vus, int player, int a) {
	if (p->tab[a] != player)
		return false;
	if (player == 1 && get_line(p, a) == p->size - 1)
		return true;
	if (player == 2 && get_column(p, a) == p->size - 1)
		return true;
	vus[a] = true;
	Stack* s = get_neighbours(p, vus, player, a);
	while (s != NULL) {
		if (isNextWinner(p, vus, player, s->val)) {
			stack_free(s);
			return true;
		}
		s = stack_pop(s);
	}
	return false;
}

bool isWinner(Plateau* p, int player) {
	bool *vus = malloc(p->size * p->size * sizeof(bool));
	if (vus == NULL)
		return false;
	for (int i = 0; i < p->size * p->size; i++)
		vus[i] = false;
	Stack* s = NULL;
	if (player == 1) {
		for (int i = 0; i < p->size; i++)
			if (p->tab[i] == player)
				s = stack_push(s, i);
	}
	else if (player == 2) {
		for (int i = 0; i < p->size; i++)
			if (p->tab[i * p->size] == player)
				s = stack_push(s, i * p->size);
	}

	while (s != NULL) {
		vus[s->val] = true;
		if (isNextWinner(p, vus, player, s->val)) {
			stack_free(s);
			free(vus);
			return true;
		}
		s = stack_pop(s);
	}

	return false;
}

int main() {
	srand(time(NULL));

	Plateau* p = plateau_create(11);

	int player = 1;
	for (int i = 0; i < p->size * p->size;i++) {
		int coup = MCTS(p, player);
		p->tab[coup] = player;
		if (isWinner(p, player)) {
			char w = player == 1 ? 'X' : 'O'; 
			printf("\n%c win !\n\n", w);
			break;
		}
		player = (player == 1) ? 2 : 1;
		plateau_print(p);
		printf("\n\n");
		system("PAUSE");
	}
	
	plateau_print(p);

	plateau_free(p);
	return 0;
}