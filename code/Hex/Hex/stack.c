#include "stack.h"
#include <stdlib.h>
#include <assert.h>

Stack* stack_create() {
	return NULL;
}

void stack_delete(Stack *s) {
	while (s != NULL)
		s = stack_pop(s);
}

Stack *stack_push(Stack* s, int val) {
	Stack* n = malloc(sizeof(Stack));
	if (n == NULL)
		return NULL;
	n->val = val;
	n->prev = s;
	if (s == NULL)
		n->size == 1;
	else
		n->size = s->size + 1;
	return n;
}

Stack* stack_pop(Stack* s) {
	if (s == NULL)
		return NULL;
	Stack* n = s->prev;
	free(s);
	return n;
}

int stack_peek(Stack* s) {
	assert(s != NULL);
	return s->val;
}



/*
* ------------------------
* ------	Tree	------
* ------------------------
*/

void tree_delete(Tree* t) {
	if (t == NULL)
		return;
	tree_delete(t->left);
	tree_delete(t->right);
	free(t);
}

void tree_add_left(Tree* t, int val) {
	Tree* n = malloc(sizeof(Tree));
	n->val = val;
	n->left = NULL;
	n->right = NULL;
	t->left = n;
}

void tree_add_right(Tree* t, int val) {
	Tree* n = malloc(sizeof(Tree));
	n->val = val;
	n->left = NULL;
	n->right = NULL;
	t->right = n;
}