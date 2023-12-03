#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <time.h>

void swap(int* tab, int i, int j) {
	int tmp = tab[i];
	tab[i] = tab[j];
	tab[j] = tmp;
}

void print_tab(int* tab, int n) {
	for (int i = 0; i < n; i++)
		printf("%d, ", tab[i]);
	printf("\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Stack {
	int val;
	struct Stack* prev;
};
typedef struct Stack Stack;

Stack* stack_create() {
	return NULL;
}

Stack* stack_push(Stack* s, int val) {
	Stack* n = malloc(sizeof(Stack)); assert(n != NULL);
	n->val = val;
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

void stack_free(Stack* s) {
	while (s != NULL)
		s = stack_pop(s);
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
				//printf(". ");
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

Stack* neighbours(Plateau* p, int player, int a) {
	Stack* s = NULL;
	int max = p->n - 1;
	int l = a - 1;
	if (get_column(p, a) > 0 && p->tab[l] == player)
		s = stack_push(s, l);
	int r = a + 1;
	if (get_column(p, a) < max && p->tab[r] == player)
		s = stack_push(s, r);
	int t = a - p->n;
	if (get_line(p, a) > 0 && p->tab[t] == player)
		s = stack_push(s, t);
	int b = a + p->n;
	if (get_line(p, a) < max && p->tab[b] == player)
		s = stack_push(s, b);
	int tr = a - p->n + 1;
	if (get_column(p, a) < max && get_line(p, a) > 0 && p->tab[tr] == player)
		s = stack_push(s, tr);
	int bl = a + p->n - 1;
	if (get_column(p, a) > 0 && get_line(p, a) < max && p->tab[bl] == player)
		s = stack_push(s, bl);
	return s;
}

int incr_for_isWinner(Plateau *p, int player, int i) {
	return player == 1 ? i + 1 : i + p->n;
}

bool existsFalse(bool* tab, int n) {
	for (int i = 0; i < n; i++)
		if (!tab[i])
			return true;
	return false;
}

bool isWinner_rec(Plateau* p, int player, int a, bool* vus) {
	if (vus[a])
		return false;
	vus[a] = true;
	for (Stack* s = neighbours(p, player, a); s != NULL; s = stack_pop(s)) {
		if ((player == 1 && get_line(p, s->val) == p->n - 1) ||
			(player == 2 && get_column(p, s->val) == p->n - 1))
			return true;
		if (isWinner_rec(p, player, s->val, vus)) {
			return true;
		}
	}
	return false;
}

bool isWinner(Plateau* p, int player) {
	bool win = false;
	bool* vus = malloc(p->size * sizeof(bool)); assert(vus != NULL);
	for (int i = 0; i < p->size; i++)
		vus[i] = false;

	for (int i = 0; existsFalse(vus, p->size) && ((player == 1 && i < p->n) || (player == 2 && i < p->size)); i = incr_for_isWinner(p, player, i)) {
		if (p->tab[i] != player)
			continue;
		if (isWinner_rec(p, player, i, vus)) {
			win = true;
			break;
		}
	}

	free(vus);
	return win;
}

/*
* renvoie le joeuur gagnant la partie
*/
int go_to_end(Plateau* p, int player, int rival, int* coups, int nbCoups) {
	if (isWinner(p, rival))
		return rival;
	
	if (nbCoups < 0)
		return -1;

	int i = rand() % nbCoups;
	int c = coups[i];
	p->tab[c] = player;
	swap(coups, i, nbCoups - 1);
	nbCoups--;

	int res = go_to_end(p, rival, player, coups, nbCoups);

	swap(coups, i, nbCoups);
	nbCoups++;
	p->tab[c] = 0;

	return res;
}

/*
* nbTests : nombre de tests par case testee
*/
int MCTS(Plateau* p, int player, int nbTests) {
	int rival = player == 1 ? 2 :1;

	int j = 0, nbCoups = 0;
	int* coups = malloc(p->size * sizeof(int)); assert(coups != NULL);
	for (int i = 0; i < p->size; i++) {
		if (p->tab[i] == 0) {
			coups[j] = i;
			nbCoups++;
			j++;
		}
	}

	int max = INT_MIN, indiceMax = 0;

	for (int i = 0; i < nbCoups; i++) {
		int tmp = 0;	//	Nombre de victoires - nombres de defaite pour le coup joue
		p->tab[coups[i]] = player;
		swap(coups, i, nbCoups - 1);	//	On inverse avec le dernier coup pour ne plus pouvoir le jouer
		nbCoups--;
		for (int j = 0; j < nbTests; j++) {
			int goEnd = go_to_end(p, rival, player, coups, nbCoups);
			if (goEnd == -1)
				return -1;
			else if (goEnd == player)
				tmp++;
			else
				tmp--;
		}
		if (tmp > max) {
			max = tmp;
			indiceMax = i;
		}
		//	Revenir a la bonne  position
		swap(coups, i, nbCoups);
		nbCoups++;
		p->tab[coups[i]] = 0;

		//printf("coups %c tmp %d\n", coups[i] + 'a', tmp);
	}

	int c = coups[indiceMax];
	free(coups);
	return c;
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
int ia(Plateau* p, int player,int niveau) {
	int coup = MCTS(p, player, niveau);
	return coup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void play(Plateau* p, int player, int a) {	//	Fonction qui place le pion a dans le plateau
	if (p->tab[a] != 0)
		return;
	p->tab[a] = player;
}

int main() {
	srand(time(NULL));

	Plateau* p = plateau_create(5);
	print_plateau(p);

	int player = 1;
	int rival = player == 1 ? 2 : 1;

	//	IA start

	if (true) {
		int coup = ia(p, rival, 10000);
		p->tab[coup] = rival;
		printf("coup : %c\n", coup + 'a');

		printf("\n");
		print_plateau(p);
		printf("\n");
	}


	// Player vs IA

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
	//while (false) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a >= 0 && a < 25) {
			play(p, player, a);
			printf("\n");

			if (isWinner(p, player)) {
				printf("\n\nWinner player !!!\n\n");
				print_plateau(p);
				printf("\n");
				break;
			}


			printf("\n");
			print_plateau(p);
			printf("\n");


			//	-------- IA	------------

			int coup = ia(p, rival, 10000);
			if (coup == -1)
				break;
			p->tab[coup] = rival;
			printf("coup : %c\n", coup + 'a');

			if (isWinner(p, rival)) {
				printf("\n\nWinner %d !!!\n\n", rival);
				print_plateau(p);
				printf("\n");
				break;
			}

			printf("\n");
			print_plateau(p);
			printf("\n");
		}
	}


	//	IA vs IA

	while (false) {
		
		// 1
		printf("player : 1\n");
		int coup = ia(p, player, 10);
		if (coup == -1)
			break;
		p->tab[coup] = player;
		printf("coup : %c\n", coup + 'a');

		if (isWinner(p, player)) {
			printf("\n\nWinner %d !!!\n\n", player);
			print_plateau(p);
			printf("\n");
			break;
		}

		printf("\n");
		print_plateau(p);
		printf("\n");

		//	2
		printf("player 2\n");

		time_t begin = time(NULL);
		coup = ia(p, rival, 100);
		time_t end = time(NULL);
		printf("time : %ld\n", (unsigned long)difftime(end, begin));
		if (coup == -1)
			break;
		p->tab[coup] = rival;
		printf("coup : %c\n", coup + 'a');

		if (isWinner(p, rival)) {
			printf("\n\nWinner %d !!!\n\n", rival);
			print_plateau(p);
			printf("\n");
			break;
		}

		printf("\n");
		print_plateau(p);
		printf("\n");
	}

	plateau_free(p);
	return 0;
}