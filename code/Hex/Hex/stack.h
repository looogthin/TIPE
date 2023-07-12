#ifndef STACK_H
#define STACK_H

/*
* Structure d'une pile
*/
struct Stack {
	int val;
	struct Stack* prev;
};
typedef struct Stack Stack;

/*
* Stack *s stack_create
* Parametre :
* Return Stack * : pointeur vers la nouvelle pile vide
* 
* Cree une nouvelle pile vide
*/
Stack* stack_create();

/*
* void stack_delete
* Parametre :
*	- Stack *s : pile a supprimer
* Return void
* 
* Supprime une pile
*/
void stack_delete(Stack *s);

/*
* Stack *stack_push
* Parametres :
*	- Stack *s : pile dans laquelle l'element doit etre ajoute
*	- int val : valeur a ajouter dans la pile
* Return Stack* : pointeur vers la nouvelle pile
* 
* Ajoute un element au sommet de la pile
*/
Stack* stack_push(Stack *s, int val);

/*
* Stack* stack_pop
* Parametre :
*	- Stack *s : pile qui doit etre depilee
* Return Stack* pointeur vers la nouvelle pile
* 
* Enleve l'element au sommet de la pile
*/
Stack* stack_pop(Stack* s);

/*
* int stack_peek
* Parametre :
*	- Stack *s : Pile dont on veut connaitre l'element
* Return int : element en haut dela pile
* 
* Renvoie l'element en haut de la pile sans l'enlever
*/
int stack_peek(Stack* s);

#endif