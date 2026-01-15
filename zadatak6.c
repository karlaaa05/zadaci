#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct article;
typedef struct article* ArticlePosition;
typedef struct article {
	char name[20];
	int quantity;
	int price;
	ArticlePosition next;
} Article;

struct receipt;
typedef struct receipt* ReceiptPosition;
typedef struct receipt {
	char date[11];
	Article headArticle;
	ReceiptPosition next;
} Receipt;

int addArticle(ArticlePosition head, ArticlePosition article);
int addReceipt(ReceiptPosition head, ReceiptPosition receipt);
int readReceipt(char* fileName, ReceiptPosition headReceipt);
int printArticles(ArticlePosition article);
int printReceipts(ReceiptPosition receipt);
int getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);
int deleteArticles(ArticlePosition articleHead);
int deleteReceipts(ReceiptPosition receiptHead);

int main()
{
	int income = 0, count = 0;
	char article[100] = { 0 };
	char startDate[20] = { 0 }, endDate[20] = { 0 };
	char currReceipt[MAX_LINE] = { 0 };
	FILE* filePointer = NULL;
	Receipt headReceipt = {
		.date = {0},
		.headArticle = NULL,
		.next = NULL
	};

	filePointer = fopen("racuni.txt", "r");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer)) {
		fgets(currReceipt, MAX_LINE, filePointer);
		currReceipt[strcspn(currReceipt, "\n")] = 0; 
		printf("%s\n", currReceipt);
		readReceipt(currReceipt, &headReceipt);
	}

	printReceipts(headReceipt.next);

	printf("\nEnter article name: ");
	if (scanf("%s", article) != 1) return SCANF_ERROR;
	printf("Enter start date (YYYY-MM-DD): ");
	if (scanf("%s", startDate) != 1) return SCANF_ERROR;
	printf("Enter end date (YYYY-MM-DD): ");
	if (scanf("%s", endDate) != 1) return SCANF_ERROR;

	getArticleInPeriod(&headReceipt, article, startDate, endDate, &income, &count);
	printf("Income of %d %s articles: %d\n", count, article, income);


	deleteReceipts(&headReceipt);
	fclose(filePointer);

	return 0;
}

//funkcija koja dodaje artikal na kraj liste
int addArticle(ArticlePosition head, ArticlePosition article) {
	ArticlePosition current = head;
	while (current->next != NULL && strcmp(current->next->name, article->name) < 0)
		current = current->next;

	article->next = current->next;
	current->next = article;
	return 0;
}

//funkcija koja dodaje racun na kraj liste sortiran po datumu 
int addReceipt(ReceiptPosition head, ReceiptPosition receipt) {
	ReceiptPosition current = head;

	while (current->next != NULL && strcmp(current->next->date, receipt->date) <= 0)
		current = current->next;

	receipt->next = current->next;
	current->next = receipt;

	return 0;
}

//funkcija koja cita racun iz datoteke i dodaje ga u listu racuna
int readReceipt(char* fileName, ReceiptPosition headReceipt) {
	FILE* filePointer = NULL;
	ReceiptPosition newReceipt = NULL;
	ArticlePosition newArticle = NULL;

	filePointer = fopen(fileName, "r");
	if (!filePointer) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (!newReceipt) {
		printf("Malloc error.\n");
		return MALLOC_ERROR;
	}

	fgets(dateString, 20, filePointer);
	dateString[strcspn(dateString, "\n")] = 0;
	strcpy(newReceipt->date, dateString);
	newReceipt->headArticle.next = NULL;

	while (!feof(filePointer)) {
		newArticle = (ArticlePosition)malloc(sizeof(Article));
		if (!newArticle) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;
		}
		if (fscanf(filePointer, "%[^,], %d, %d", newArticle->name,&newArticle->quantity, &newArticle->price) != 3) {
			printf("fscanf error\n");
			return SCANF_ERROR;
		}

		addArticle(&newReceipt->headArticle, newArticle);
	}
	addReceipt(headReceipt, newReceipt);
	fclose(filePointer);
	return 0;
}

//funkcija koja ispisuje sve artikle iz liste
int printArticles(ArticlePosition current) {
	while (current) {
		printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
		current = current->next;
	}
	return 0;
}

//funkcija koja ispisuje sve racune iz liste
int printReceipts(ReceiptPosition current) {
	while (current) {
		printf("Receipt date: %s\n", current->date);
		printArticles(current->headArticle.next);
		current = current->next;
	}
	return 0;
}

int getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count) {
	ArticlePosition currentArticle = NULL;

	while (currentReceipt!=NULL && strcmp(currentReceipt->date, startDate) < 0)
		currentReceipt = currentReceipt->next;

	while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {
		currentArticle = currentReceipt->headArticle.next;

		while (currentArticle != NULL) {
			if (strcmp(currentArticle->name, article) == 0) {
				*income += currentArticle->quantity * currentArticle->price;
				*count += currentArticle->quantity;
			}
			currentArticle = currentArticle->next;
		}
		currentReceipt = currentReceipt->next;
	}

return EXIT_SUCCESS;
}


//funkcija koja brise sve artikle iz liste
int deleteArticles(ArticlePosition arcticleHead) {
	ArticlePosition temp = NULL;
	while (arcticleHead->next != NULL) {
		temp = arcticleHead->next;
		arcticleHead->next = temp->next;
		free(temp);
	}
	return 0;
}

int deleteReceipts(ReceiptPosition receiptHead) {
	ReceiptPosition temp = NULL;

	while (receiptHead->next != NULL) {
		temp = receiptHead->next;
		receiptHead->next = temp->next;

		deleteArticles(&temp->headArticle);
		free(temp);
	}
	return 0;
}