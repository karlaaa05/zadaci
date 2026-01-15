#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1;
#define SCANF_ERROR -2
#define MALLOC_ERROR -3


struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
	int population;
	char name[20];
	CityTreePosition left;
	CityTreePosition right;
} CityTree;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
	char name[20];
	CountryListPosition next;
	CityTreePosition cities;
} CountryList;

typedef struct {
	int size;
	CountryList hashItem[11];
} HashTable;

int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity);
int hash(char*, int);
CountryListPosition findInHashTable(HashTable*, char*);
int printCitiesWithGreaterPopulation(CityTreePosition, int);

int main()
{
	HashTable hashTable = {
		.size = 11,
		.hashItem = {
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL },
			{.name = {0}, .next = NULL, .cities = NULL }
		}
	};
	CountryList countryListHead = { .name = { 0 }, .next = NULL };
	char country[20] = { 0 }, countryFilePath[24] = { 0 }, city[20] = { 0 }, countryNameChoice[20] = { 0 };
	int population = 0, populationChoice = 0;
	FILE* countryFilePointer = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("drzave.txt", "r");
	if (filePointer == NULL) {
		printf("Could not open file 'drzave.txt'\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer))
	{
		if (fscanf(filePointer, "%[^,], %s ", country, countryFilePath) != 2) {
			printf("fscanf error.\n");
			return SCANF_ERROR;
		}

		CountryListPosition newCountryList = NULL;
		newCountryList = (CountryListPosition)malloc(sizeof(CountryList));
		if (newCountryList == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryList->name, country);
		newCountryList->cities = NULL;
		newCountryList->next = NULL;

		int countryHash = hash(country, hashTable.size);
		if (hashTable.hashItem[countryHash].next == NULL) {
			hashTable.hashItem[countryHash].next = newCountryList;
		}
		else {
			CountryListPosition current = &hashTable.hashItem[countryHash];
			while (current->next != NULL && strcmp(current->next->name, country) < 0)
				current = current->next;
			newCountryList->next = current->next;
			current->next = newCountryList;
		}

		countryFilePointer = fopen(countryFilePath, "r");
		while (!feof(countryFilePointer))
		{
			if (fscanf(countryFilePointer, "%[^,], %d ", city, &population) != 2) {
				printf("fscanf error.\n");
				return SCANF_ERROR;
			}

			CityTreePosition newCityTree = NULL;
			newCityTree = (CityTreePosition)malloc(sizeof(CityTree));
			if (newCityTree == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}
			strcpy(newCityTree->name, city);
			newCityTree->population = population;
			newCountryList->cities = addCityToTree(newCountryList->cities, newCityTree);
		}

		fclose(countryFilePointer);
	}

	fclose(filePointer);

	while (1)
	{
		printf("Choose country: ");
		if (scanf(" %19[^\n]", countryNameChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}
		if ((strlen(countryNameChoice) > 0) && (countryNameChoice[strlen(countryNameChoice) - 1] == '\n'))
			countryNameChoice[strlen(countryNameChoice) - 1] = '\0';
		if (!strcmp(countryNameChoice, "0")) break;

		CountryListPosition countryChoice = findInHashTable(&hashTable, countryNameChoice);
		if (countryChoice == NULL) {
			printf("No such country.\n");
			continue;
		}

		printf("Enter population: ");
		if (scanf(" %d", &populationChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}

		printf("Cities in %s with population greater than %d:\n", countryNameChoice, populationChoice);
		printCitiesWithGreaterPopulation(countryChoice->cities, populationChoice);

	}



	return EXIT_SUCCESS;
}

int addCountryToList(CountryListPosition current, CountryListPosition newCountry)
{
	while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0) current = current->next;

	newCountry->next = current->next;
	current->next = newCountry;

	return EXIT_SUCCESS;
}
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity)
{
	if (current == NULL) {
		current = newCity;
		current->left = NULL;
		current->right = NULL;
	}

	if (newCity->population < current->population) current->left = addCityToTree(current->left, newCity);
	else if (newCity->population > current->population) current->right = addCityToTree(current->right, newCity);
	else {
		if (strcmp(newCity->name, current->name) < 0) current->left = addCityToTree(current->left, newCity);
		else if (strcmp(newCity->name, current->name) > 0) current->right = addCityToTree(current->right, newCity);
	}

	return current;
}

int hash(char* country, int tableSize)
{
	int i = 0, sum = 0;

	for (i = 0; i < 5; i++) sum += country[i];

	return sum % tableSize;
}
CountryListPosition findInHashTable(HashTable* hashTable, char* countryName)
{
	CountryListPosition current = hashTable->hashItem[hash(countryName, hashTable->size)].next;

	while (current != NULL && strcmp(current->name, countryName)) current = current->next;

	return current;
}

int printCitiesWithGreaterPopulation(CityTreePosition root, int population)
{
	if (root == NULL) return NULL;

	if (root->population > population) printf("\t%s, %d\n", root->name, root->population);

	printCitiesWithGreaterPopulation(root->left, population);
	printCitiesWithGreaterPopulation(root->right, population);
}