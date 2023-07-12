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