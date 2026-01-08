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

struct cityList;
typedef struct cityList* CityListPosition;
typedef struct cityList {
	int population;
	char name[20];
	CityListPosition next;
} CityList;

struct countryTree;
typedef struct countryTree* CountryTreePosition;
typedef struct countryTree {
	char name[20];
	CountryTreePosition left;
	CountryTreePosition right;
	CityList cityHead;
} CountryTree;

int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry);
int addCityToList(CityListPosition current, CityListPosition newCity);
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity);
int printCityTree(CityTreePosition current);
int printCountryList(CountryListPosition current);
int printCityList(CityListPosition current);
int printCountryTree(CountryTreePosition current);
CountryTreePosition findCountryByName(CountryTreePosition current, char* name);

int main()
{
	CountryList countryListHead = { .name = { 0 }, .next = NULL };
	CountryTreePosition countryTree = NULL;
	CountryTreePosition countryChoice = NULL;
	CityListPosition firstCity = NULL;
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

		CountryTreePosition newCountryTree = NULL;
		newCountryTree = (CountryTreePosition)malloc(sizeof(CountryTree));
		if (newCountryTree == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryTree->name, country);
		newCountryTree->cityHead = (CityList){ .name = { 0 },  .population = 0, .next = NULL };

		addCountryToList(&countryListHead, newCountryList);
		countryTree = addCountryToTree(countryTree, newCountryTree);

		countryFilePointer = fopen(countryFilePath, "r");
		while (!feof(countryFilePointer)) {
			if (fscanf(countryFilePointer, "%[^,], %d ", city, &population) != 2) {
				printf("fscanf error.\n");
				return SCANF_ERROR;
			}

			CityListPosition newCityList = NULL;
			newCityList = (CityListPosition)malloc(sizeof(CityList));
			if (newCityList == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}
			strcpy(newCityList->name, city);
			newCityList->population = population;

			CityTreePosition newCityTree = NULL;
			newCityTree = (CityTreePosition)malloc(sizeof(CityTree));
			if (newCityTree == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}
			strcpy(newCityTree->name, city);
			newCityTree->population = population;

			addCityToList(&newCountryTree->cityHead, newCityList);
			newCountryList->cities = addCityToTree(newCountryList->cities, newCityTree);
		}

		fclose(countryFilePointer);
	}

	fclose(filePointer);

	printf("Country list:\n---------------\n");
	printCountryList(countryListHead.next);
	printf("\nCountry tree:\n---------------\n");
	printCountryTree(countryTree);

	while (1)
	{
		printf("\nChoose country: ");
		if (scanf(" %19[^\n]", countryNameChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}
		if ((strlen(countryNameChoice) > 0) && (countryNameChoice[strlen(countryNameChoice) - 1] == '\n'))
			countryNameChoice[strlen(countryNameChoice) - 1] = '\0';
		if (!strcmp(countryNameChoice, "0")) break;

		countryChoice = findCountryByName(countryTree, countryNameChoice);
		if (countryChoice == NULL) {
			printf("No such country.\n");
			continue;
		}

		printf("Enter some population to find cities with greater population: ");
		if (scanf(" %d", &populationChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}

		firstCity = countryChoice->cityHead.next; // First city with population greater than population choice
		while (firstCity != NULL && firstCity->population <= populationChoice)
			firstCity = firstCity->next;

		if (firstCity == NULL) printf("No such city.\n");
		else {
			printf("Cities in %s with population greater than %d:\n", countryNameChoice, populationChoice);
			printCityList(firstCity);
		}

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

CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry)
{
	if (current == NULL) {
		current = newCountry;
		current->left = NULL;
		current->right = NULL;
	}

	if (strcmp(newCountry->name, current->name) < 0) current->left = addCountryToTree(current->left, newCountry);
	else if (strcmp(newCountry->name, current->name) > 0) current->right = addCountryToTree(current->right, newCountry);

	return current;
}

int addCityToList(CityListPosition current, CityListPosition newCity)
{
	while (current->next != NULL && newCity->population > current->next->population) current = current->next;
	if (current->next != NULL && current->next->population == newCity->population) {
		while (current->next != NULL && strcmp(newCity->name, current->next->name) > 0) current = current->next;
	}

	newCity->next = current->next;
	current->next = newCity;

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

int printCityTree(CityTreePosition current)
{
	if (current == NULL) return EXIT_SUCCESS;

	printCityTree(current->left);
	printf("\t%s, %d\n", current->name, current->population);
	printCityTree(current->right);

	return EXIT_SUCCESS;
}
int printCountryList(CountryListPosition current)
{
	while (current != NULL) {
		printf("%s\n", current->name);
		printCityTree(current->cities);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printCityList(CityListPosition current)
{
	while (current != NULL) {
		printf("\t%s, %d\n", current->name, current->population);
		current = current->next;
	}

	return EXIT_SUCCESS;
}
int printCountryTree(CountryTreePosition current)
{
	if (current == NULL) return EXIT_SUCCESS;

	printCountryTree(current->left);
	printf("%s\n", current->name);
	printCityList(current->cityHead.next);
	printCountryTree(current->right);

	return EXIT_SUCCESS;
}

CountryTreePosition findCountryByName(CountryTreePosition current, char* name)
{
	if (current == NULL) return NULL;

	if (strcmp(name, current->name) > 0) return findCountryByName(current->right, name);
	else if (strcmp(name, current->name) < 0) return findCountryByName(current->left, name);

	return current;
}