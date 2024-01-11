#include "ia.h"
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

/*
* renvoie le joeuur gagnant la partie
*/
int go_to_end(Game *g, int player, int rival, int* coups, int nbCoups) {
	if (isWinner(g, rival))
		return rival;

	if (nbCoups < 0)
		return -1;

	int i = rand() % nbCoups;
	int c = coups[i];
	g->plateau[c] = player;
	swap(coups, i, nbCoups - 1);
	nbCoups--;

	int res = go_to_end(g, rival, player, coups, nbCoups);

	swap(coups, i, nbCoups);
	nbCoups++;
	g->plateau[c] = 0;

	return res;
}

/*
* nbTests : nombre de tests par case testee
*/
int MCTS(Game *g, int player, int nbTests) {
	int rival = player == 1 ? 2 : 1;

	int j = 0, nbCoups = 0;
	int* coups = malloc(g->size * sizeof(int)); assert(coups != NULL);
	for (int i = 0; i < g->size; i++) {
		if (g->plateau[i] == 0) {
			coups[j] = i;
			nbCoups++;
			j++;
		}
	}

	int max = INT_MIN, indiceMax = 0;

	for (int i = 0; i < nbCoups; i++) {
		int tmp = 0;	//	Nombre de victoires - nombres de defaite pour le coup joue
		g->plateau[coups[i]] = player;
		swap(coups, i, nbCoups - 1);	//	On inverse avec le dernier coup pour ne plus pouvoir le jouer
		nbCoups--;
		for (int j = 0; j < nbTests; j++) {
			int goEnd = go_to_end(g, rival, player, coups, nbCoups);
			if (goEnd == -1)
				return -1;
			else if (goEnd == player)
				tmp += 1;
			else
				tmp -= 1;
		}
		if (tmp > max) {
			max = tmp;
			indiceMax = i;
		}
		//	Revenir a la bonne  position
		swap(coups, i, nbCoups);
		nbCoups++;
		g->plateau[coups[i]] = 0;

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
*	- Game *g
*	- Graph *g : Graph du joueur actuelle
*	- Graph *ga : Graph du joueur adverse
* Return (int) : Le coup que l'ia va jouer
*
* Determine le meilleur coup possible a jouer pour l'IA
*/
int ia_play(Game *g, IA *ia) {
	int niveau;
	if (ia->p->num == 1) niveau = 1000;
	else niveau = 1000;
	time_t begin = time(NULL);
	int coup = MCTS(g, ia->p->num, niveau);
	time_t end = time(NULL);
	printf("niveau : %d, time : %ld\n", niveau, (unsigned long)difftime(end, begin));
	return coup;
}
