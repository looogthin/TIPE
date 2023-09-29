#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
	int* tab;
	int* rg;
	int n;
} UnionFind;

UnionFind* uf_create(int n) {
	assert(n > 0);
	UnionFind* uf = malloc(sizeof(UnionFind));
	uf->tab = malloc(n * sizeof(int));
	uf->rg = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		uf->tab[i] = -1;
		uf->rg[i] = 0;
	}
	uf->n = n;
	return uf;
}

int uf_find(UnionFind* uf, int a) {
	assert(a >= 0 && a < uf->n);
	int tmp = a;
	int rg = 0;
	while (uf->tab[tmp] != -1)
		tmp = uf->tab[tmp];
	if (tmp == a)
		return tmp;

	while (uf->tab[a] != tmp) {
		int i = uf->tab[a];
		uf->tab[a] = tmp;
		a = i;
		rg++;
	}
	uf->rg[tmp] = rg;
	return tmp;
}

void uf_union(UnionFind* uf, int a, int b) {
	assert(a >= 0 && a < uf->n);
	assert(b >= 0 && b < uf->n);
	a = uf_find(uf, a);
	b = uf_find(uf, b);
	if (a == b)
		return;
	else if (uf->rg[a] < uf->rg[b])
		uf->tab[a] = b;
	else
		uf->tab[b] = a;
}

void uf_free(UnionFind* uf) {
	assert(uf != NULL && uf->tab != NULL && uf->rg != NULL);
	free(uf->tab);
	free(uf->rg);
	free(uf);
}

void uf_print(UnionFind* u) {
	for (int i = 0; i < u->n; i++)
		printf("%d, ", u->tab[i]);
	printf("\n");

	for (int i = 0; i < u->n; i++) {
		if (u->tab[i] == -1) {
			for (int j = 0; j < u->n; j++) {
				if (uf_find(u, j) == i)
					printf("%c, ", j + 'a');
			}
			printf("\n");
		}
	}
}

int* plateau;
int nbCases;
int size;
UnionFind *grp1, *grp2;

int get_column(int a) {
	return a % nbCases;
}

int get_line(int a) {
	return a / nbCases;
}

void print_plateau() {
	for (int i = 0; i < nbCases; i++) {
		for (int j = 0; j < nbCases; j++) {
			if (plateau[i * nbCases + j] == 0)
				printf("%c ", i * nbCases + j + 'a');
			else
				printf("%d ", plateau[i * nbCases + j]);
		}
		printf("\n");
	}
}

void union_neighbours(int p, int a) {
	UnionFind* u = p == 1 ? grp1 : grp2;
	int max = nbCases - 1;
	int l = a - 1;
	if (get_column(a) > 0 && plateau[l] == p)
		uf_union(u, a, l);
	int r = a + 1;
	if (get_column(a) < max && plateau[r] == p)
		uf_union(u, a, r);
	int t = a - nbCases;
	if (get_line(a) > 0 && plateau[t] == p)
		uf_union(u, a, t);
	int b = a + nbCases;
	if (get_line(a) < max && plateau[b] == p)
		uf_union(u, a, b);
	int tr = a - nbCases + 1;
	if (get_column(a) < max && get_line(a) > 0 && plateau[tr] == p)
		uf_union(u, a, tr);
	int bl = a + nbCases - 1;
	if (get_column(a) > 0 && get_line(a) < max && plateau[bl] == p)
		uf_union(u, a, bl);
}

void play(int player, int a) {
	if (plateau[a] != 0)
		return;
	plateau[a] = player;
	union_neighbours(player, a);
	print_plateau();
	printf("\n");
	uf_print(grp1);
}

int main() {
	nbCases = 5;
	size = nbCases * nbCases;
	plateau = malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		plateau[i] = 0;
	print_plateau();

	grp1 = uf_create(size);
	grp2 = uf_create(size);

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		int a = c - 'a';
		if (a >= 0 && a < 25)
			play(1, a);

		printf("\n");
	}


	uf_free(grp1);
	uf_free(grp2);
	free(plateau);
	return 0;
}