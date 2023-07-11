#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

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

void isWinner(Plateau* p, int player) {

}

int main() {
	srand(time(NULL));

	Plateau* p = plateau_create(11);

	int player = 1;
	for (int i = 0; i < 121;) {
		int a = rand() % 122;
		if (p->tab[a] == 0) {
			p->tab[a] = player;
			player = (player == 1) ? 2 : 1;
			i++;
		}
	}

	plateau_print(p);

	plateau_free(p);
	return 0;
}