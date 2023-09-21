#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int nbCases = 5;
const int size = 25;
const int limitRefreshSizeGroupPawns = 5;	// taille avant de reallouer un plus gros espace pour un groupe de pions

struct Stack {
	int val;
	struct Stack* prev;
};
typedef struct Stack Stack;

Stack* stack_push(Stack* s, int val) {
	Stack* n = malloc(sizeof(Stack));
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

struct PawnsGroup {
	int size;
	int* tab;
	int mallocsize;
};
typedef struct PawnsGroup PawnsGroup;

void pawnsGroup_free(PawnsGroup* g) {
	if (g == NULL)
		return;
	if (g->tab != NULL)
		free(g->tab);
	free(g);
}

struct Player {
	int num;
	int nbGroups;
	PawnsGroup* groups;
};
typedef struct Player Player;

void player_free(Player *p) {
	if (p == NULL)
		return;
	for (int i=0; i < p->nbGroups; i++)
		pawnsGroup_free(&p->groups[i]);
	free(p);
}

struct Pawn {
	Player* p;
	int grp;
};
typedef struct Pawn Pawn;

Player *player, *ia;
Pawn* plateau;

void print_plateau() {
	for (int i = 0; i < nbCases; i++) {
		for (int j = 0; j < nbCases; j++) {
			int c = i * nbCases + j;
			if (plateau[c].p == NULL)
				printf("%c ", c + 'a');
			else
				printf("%d ", plateau[i * nbCases + j].p->num);
		}
		printf("\n");
	}
}

int get_line(int a) {
	return a / nbCases;
}

int get_column(int a) {
	return a % nbCases;
}

/*
* les groupes doivent etre differents et appartenir au joueur
* Tous les pions de b vont dans a puis b est "supprimer"
*/
void merge_groups(Player *p, int a, int b) {
	printf("merge groups\n");
	int sizeA = p->groups[a].size;
	p->groups[a].size += p->groups[b].size;
	if (p->groups[a].size >= p->groups[a].mallocsize) {
		printf("realloc merge group\n");
		while (p->groups[a].mallocsize <= p->groups[a].size)
			p->groups[a].mallocsize += limitRefreshSizeGroupPawns;
		int *tmp = realloc(p->groups[a].tab, p->groups[a].mallocsize);
		assert(tmp != NULL);
		p->groups[a].tab = tmp;
		printf("fin realloc merge groups\n");
	}

	for (int i = 0; i < p->groups[b].size; i++) {
		p->groups[a].tab[sizeA + i] = p->groups[b].tab[i];
		plateau[p->groups[b].tab[i]].grp = a;
	}
	p->groups[b].size = 0;
	printf("end merge groups\n");
}

Stack* get_neighbours(Player *p, int a) {
	Stack* s = NULL;
	int max = nbCases - 1;
	int l = a - 1;
	if (get_column(a) > 0 && plateau[l].p == p)
		s = stack_push(s, l);
	int r = a + 1;
	if (get_column(a) < max && plateau[r].p == p)
		s = stack_push(s, r);
	int t = a - nbCases;
	if (get_line(a) > 0 && plateau[t].p == p)
		s = stack_push(s, t);
	int b = a + nbCases;
	if (get_line(a) < max && plateau[b].p == p)
		s = stack_push(s, b);
	int tr = a - nbCases + 1;
	if (get_column(a) < max && get_line(a) > 0 && plateau[tr].p == p)
		s = stack_push(s, tr);
	int bl = a + nbCases - 1;
	if (get_column(a) > 0 && get_line(a) < max && plateau[bl].p == p)
		s = stack_push(s, bl);
	return s;
}

int set_groups(Player *p, int a) {
	Stack* s = get_neighbours(p, a);
	int numGrp = -1;

	if (s == NULL) {	//	Nouveau groupe
		PawnsGroup grp = { 1, malloc(limitRefreshSizeGroupPawns * sizeof(int)), limitRefreshSizeGroupPawns };
		grp.tab[0] = a;
		p->groups[p->nbGroups] = grp;
		numGrp = p->nbGroups;
		p->nbGroups++;
		plateau[a].grp = numGrp;
	}
	
	else {
		numGrp = plateau[s->val].grp;
		PawnsGroup* grp = &p->groups[numGrp];
		if (grp->size >= grp->mallocsize) {
			printf("realloc\n");
			grp->mallocsize += limitRefreshSizeGroupPawns;
			int* tmp = realloc(grp->tab, grp->mallocsize * sizeof(int));
			assert(tmp != NULL);
			grp->tab = tmp;
		}
		grp->tab[grp->size] = a;
		grp->size++;
		plateau[a].grp = numGrp;

		//	Plusieurs voisins
		for (; s != NULL; s = stack_pop(s))
			if (numGrp != plateau[s->val].grp)
				merge_groups(p, numGrp, plateau[s->val].grp); 
	}

	return numGrp;
}

void print_groups(Player* p) {
	printf("Groups :\n");
	for (int i = 0; i < p->nbGroups; i++) {
		PawnsGroup grp = p->groups[i];
		for (int j = 0; j < grp.size; j++) {
			if (grp.tab[j] >= 0 && grp.tab[j] <= 25)
				printf("%c, ", grp.tab[j] + 'a');
			else
				printf("%d, ", grp.tab[j]);
		}
		printf("\n");
	}
}

int ia_play() {
	return -1;
}

int play(int coup) {
	if (coup < 0 || coup > size || plateau[coup].p != NULL)
		return -1;

	plateau[coup].p = player;
	set_groups(player, coup);

	printf("\n");
	print_plateau();
	printf("\n");
	print_groups(player);

	return 0;
}

int main() {
	Player playertmp = { 1, 0, malloc(size * sizeof(int)) };
	player = &playertmp;
	Player iatmp = { 2, 0, malloc(size * sizeof(int)) };
	ia = &iatmp;

	Pawn pawnEmpty = { NULL, -1 };
	plateau = malloc(size * sizeof(Pawn));
	for (int i = 0; i < size; i++)
		plateau[i] = pawnEmpty;
	print_plateau();
	printf("\n");

	char c;
	while (scanf_s("%c", &c, 2) != EOF) {
		if (c == '5')
			break;
		if (c == 'z') {
			ia_play();
			continue;
		}
		int p = play(c - 'a');
	}

	player_free(player);
	player_free(ia);
	return 0;
}