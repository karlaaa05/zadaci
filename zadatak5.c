#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define INVALID_INPUT 2
#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1aj 
#define FILE_OPEN_ERROR -2
#define SSCANF_ERROR -3

//jedan element stoga
typedef struct stackItem {
	float value;
	struct stackItem* next;
} StackItem; 

//struktura koja pamti pokazivac na vrh stoga
typedef struct {
	StackItem* top;
} Stack;

int push(Stack* stack, float x); // push-dodajemo broj na vrh stoga
int pop(Stack* stack, float* x); //pop-uzimamo broj s vrha

int main() {
	int size = 0;
	float firstOperand = 0, secondOperand = 0; //to su dva broja koja skinemo sa stog kada naidemo na operand
	float result = 0;
	char currItem[10] = { 0 }; //string u koji upisujemo trenutni token
	char postfix[MAX_LINE] = { 0 }; //cijeli izraz koji procitamo iz datoteke
	char* postfixPointer = postfix; // pokazivac koji se pomice po izrazu da citamo redom svaki token

	FILE* fp = NULL;
	Stack stack = { .top = NULL };  //stog je na pocetku prazan
	fp = fopen("postfix.txt", "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke.\n");
		return FILE_OPEN_ERROR;
	}
	fgets(postfix, MAX_LINE, fp);
	fclose(fp);

	printf("Postfix:%s\n", postfix);

	while (strlen(postfixPointer) > 0) {
		if (sscanf(postfixPointer, " %s %n", currItem, &size) != 1) {
			printf("Greska pri citanju iz stringa.\n");
			return SSCANF_ERROR;
		}
		//ako je broj
		if (currItem[0] >= '0' && currItem[0] <= '9') {
			if (push(&stack, atof(currItem)) != 0) //atof-ASCII to float
				return MALLOC_ERROR;
		}
		//ako je operator 
		else if (currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*' || currItem[0] == '/') {
			if (pop(&stack, &secondOperand) || pop(&stack, &firstOperand)) {
				printf("Prazan stog.\n");
				return EMPTY_STACK;
			}
			switch (currItem[0]) {
			case '+':
				if (push(&stack, firstOperand + secondOperand))
					return MALLOC_ERROR;
				break;
			case '-':
				if (push(&stack, firstOperand - secondOperand))
					return MALLOC_ERROR;
				break;

			case '*':
				if (push(&stack, firstOperand * secondOperand))
					return MALLOC_ERROR;
				break;

			case '/':
				if (push(&stack, firstOperand / secondOperand))
					return MALLOC_ERROR;
				break;
			}
		}

		else {
			printf("Neispravan unos.\n");
			return INVALID_INPUT;
		}
		postfixPointer = postfixPointer + size;
	}
	if (pop(&stack, &result)) {
		printf("Prazan stog.\n");
		return EMPTY_STACK;
	}
	printf("Rezultat: %g\n", result);
	return 0;
}

//funkcija koja dodaje broj na vrh stacka
int push(Stack* stack, float x) {
	StackItem* item = NULL; //pokazivac na novi element
	item = (StackItem*)malloc(sizeof(StackItem));
	if (item == NULL) return MALLOC_ERROR;


item->value = x;
item->next = stack->top; 
stack->top = item;

return EXIT_SUCCESS; }

//funkcija koja skida element s vrha i njegovu vrijednost stavlja u *x
int pop(Stack* stack, float* x) {
	StackItem* top;
	top = stack-> top;
	if (top == NULL) return EMPTY_STACK;

	*x = top->value;
	stack->top = top->next;
	free(top);
	return EXIT_SUCCESS;
}