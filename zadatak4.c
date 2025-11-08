#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2048
#define FILE_ERROR -1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct monomial {
	int coef;
	int exp;
	struct monomial* next;
} Mono;

int strToPoly (Mono*, char*);
int addMonoToPoly(Mono*, Mono*);
int printMono(Mono*);
int printPoly(Mono*);
int deletePoly(Mono*);
int polySum(Mono*, Mono*, Mono*);
int polyProduct(Mono*, Mono*, Mono*);

int main() {

	Mono poly1Head = { .coef = 0, .exp = 0, .next = NULL }; //glava liste prvog polinoma
	Mono poly2Head = {.coef = 0, .exp = 0, .next = NULL }; // glava liste drugog polinoma
	Mono sumHead = {.coef = 0, .exp = 0, .next = NULL };  //glava liste za zbroj polinoma
	Mono productHead = {.coef = 0, .exp = 0, .next = NULL }; //glava liste za produkt polinoma

	int i, size = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* fp = NULL;
	fp = fopen("polynomials.txt", "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke.\n");
		return FILE_ERROR;
	}
	//cita prvi red i pretvara ga u poly1
	fgets(buffer, MAX_LINE, fp);
	strToPoly(&poly1Head, buffer);

	//cita drugi red i pretvara ga u poly2
	fgets(buffer, MAX_LINE, fp);
	strToPoly(&poly2Head, buffer);

	printf("Prvi polinom: ");
	printPoly(poly1Head.next);
	printf("Drugi polinom: ");
	printPoly(poly2Head.next);

	polySum(&sumHead, poly1Head.next, poly2Head.next);
	printf("Zbroj polinoma: ");
	printPoly(sumHead.next);

	polyProduct(&productHead, poly1Head.next, poly2Head.next);
	printf("Produkt polinoma: ");
	printPoly(productHead.next);

	fclose(fp);

	deletePoly(&poly1Head);
	deletePoly(&poly2Head);
	deletePoly(&sumHead);
	deletePoly(&productHead);

	return EXIT_SUCCESS; }


//funkcija koja dodaje monom u polinom
int addMonoToPoly(Mono* head, Mono* mono) { //mono-pokazivac na monom koji se dodaje
	Mono* prev = head; //pokazivac na prethodni element, iza kojeg se dodaje novi monom
	Mono* temp = NULL; 

	//provjera ako je koeficijent 0, ako je obrisemo ga
	if (mono->coef == 0) {
		free(mono);
		return EXIT_SUCCESS;
	}

	//sortiramo po eksponentima,od veceg prema manjem, trazimo prvi monom s eksp manjim ili jednakim eksp novog monoma
	while (prev->next != NULL && prev->next->exp > mono->exp)
		prev = prev->next;

	//ako postoji monom s istim eksponentom, zbrojimo koeficijente
	if (prev->next != NULL && mono->exp == prev->next->exp) {
		prev->next->coef = prev->next->coef + mono->coef;
		free(mono);


		//ako je koeficijent postao 0, izbrisemo monom
		if (prev->next->coef == 0) {
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}
	
	
	else {
		//umetnemo novi monom nakon prethodnog
		mono->next = prev->next;
		prev->next = mono;
	}
	return EXIT_SUCCESS;
}

//funkcija koja pretvara string u polinom
		int strToPoly(Mono * head, char* str) { //str-string koji sadrzi polinom,redak iz datoteke
			int size = 0; //broj znakova koje sscanf procita iz stringa
			Mono* mono = NULL; //pokazivac na novi monom

			while (strlen(str) > 0) {
				mono = (Mono*)malloc(sizeof(Mono));

				if (mono == NULL) {
					printf("Greska pri alokaciji memorije.\n");
					return MALLOC_ERROR;
				}

				if (sscanf(str, "%dx^%d %n", &mono->coef, &mono->exp, &size) != 2) {
					return SCANF_ERROR;
				}
				
				addMonoToPoly(head, mono);
				str = str + size;
			}
			return EXIT_SUCCESS;
		}

//funkcija za ispis jednog monoma
		int printMono(Mono* mono) { //mono-pokazivac na monom koji se ispisuje

			mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);

			if (mono->exp == 1) printf("x ");

			else if (mono->exp != 0) printf("x^%d ", mono->exp);

			return EXIT_SUCCESS;
		}

//funkcija za ispis polinoma
int printPoly(Mono* current) {  //current-pokazivac na prvi monom u polinomu
			
			if (current != NULL) {
				printMono(current);
				current = current->next;
			}

			while (current != NULL) {
				if (current->coef > 0)
					printf("+ ");
				printMono(current);
				current = current->next;
			}
			printf("\n");
			return EXIT_SUCCESS;
		}

//funkcija za brisanje polinoma
int deletePoly(Mono* head) {
	Mono* temp = NULL;

	while (head->next != NULL) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
	return EXIT_SUCCESS;
}

//funkcija za zbrajanje dva polinoma
int polySum(Mono* sum, Mono* poly1, Mono* poly2) {
	Mono* temp = NULL;

	while (poly1 != NULL) { //poly1 pokazivac na trenutni monom prvog polinoma
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		//kopiramo monom u temp
		temp->coef = poly1->coef;
		temp->exp = poly1->exp;
		addMonoToPoly(sum, temp);
		poly1 = poly1->next;
	}

	while (poly2 != NULL) {
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		temp->coef = poly2->coef;
		temp->exp = poly2->exp;
		addMonoToPoly(sum, temp);
		poly2 = poly2->next;
	}
	return EXIT_SUCCESS;
}

//funkcija za mnozenje dva polinoma
int polyProduct(Mono* product, Mono* poly1, Mono* poly2) { 
	Mono* temp = NULL;
	Mono* poly2first = poly2;

	while (poly1 != NULL) {  //svaki monom iz poly1 pomnozimo sa svakim monomom iz poly2
		poly2 = poly2first; //za svako prolazenje vanjske petlje,vratimo poly2 na pocetak
		while (poly2 != NULL) {
			temp = (Mono*)malloc(sizeof(Mono));
			temp->coef = poly1->coef * poly2->coef;    //mnozimo koeficijente
			temp->exp = poly1->exp + poly2->exp;      //zbrajamo eksponente
			addMonoToPoly(product, temp);             //dodajemo novi monom u produkt
			poly2 = poly2->next;
		}
		poly1 = poly1->next;
	}

	return EXIT_SUCCESS;
}
