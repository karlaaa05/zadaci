#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH 50
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct directory;
typedef struct directory* DirectoryPosition; //pokazivac na direktorij
typedef struct directory {
	char name[MAX_LENGTH];
	DirectoryPosition subdirectoryPosition; //pokazivac na poddirektorij
	DirectoryPosition next; //pokazivac na sljedeci direktorij
}Directory;

struct levelStack;
typedef struct levelStack* LevelStackPosition; //pokazivac na stek
typedef struct levelStack {
	DirectoryPosition directoryLevel; 
	LevelStackPosition next; //pokazivac na sljedeci element steka
}LevelStack;

DirectoryPosition createDirectory(char* directoryName);
int push(LevelStackPosition, DirectoryPosition);
int pop(LevelStackPosition);
int addDirectory(DirectoryPosition, DirectoryPosition);
int changeDirectory(LevelStackPosition, char*);
int printDirectory(DirectoryPosition, int);
int deleteDirectory(DirectoryPosition headDirectory);
int deleteStack(LevelStackPosition headLevelStack);

int main() {
	char input = 0; 
	char directoryName[MAX_LENGTH] = { 0 };
	Directory headDirectory = { .name = {0}, .subdirectoryPosition = NULL, .next = NULL };
	DirectoryPosition rootDirectory = NULL;  //pokazivac na korijenski direktorij
	LevelStack headLevelStack = { .directoryLevel = NULL, .next = NULL };

	rootDirectory = createDirectory("C:");
	if (rootDirectory == NULL) return MALLOC_ERROR;

	headDirectory.next = rootDirectory; //head nije pravi direktorij,samo pokazivac na prvi
	push(&headLevelStack, rootDirectory); //stavljamo korijenski na stek

	printf("1 -make directory\n2 -change directory to <dir>\n3 -go to parent directory\n4- view directory content\n5- end program\n");

	do {
		printf("Enter option: ");

		if (scanf(" %c", &input) != 1) return SCANF_ERROR;

		switch (input) {
		case '1': //make directory
			printf("md ");
			if (scanf("%s", directoryName) != 1) return SCANF_ERROR;
			DirectoryPosition newDirectory = createDirectory(directoryName);
			if (newDirectory == NULL) return MALLOC_ERROR;
			addDirectory(headLevelStack.next->directoryLevel, newDirectory);
			printf("Created directory %s\n", directoryName);
			break;

		case '2': //change directory
			printf("cd ");
			if (scanf("%s", directoryName) != 1) return SCANF_ERROR;
			changeDirectory(&headLevelStack, directoryName);
			break;

		case '3': //cd..- go to parent directory
			if (pop(&headLevelStack)) printf("Already in root directory.\n");
			else printf("You are now in parent directory.\n");
			break;

		case '4': //view directory content
			if (headLevelStack.next == NULL) {
				printf("No current directory.\n");
				break;
			}
			printf("Printing content of %s:\n", headLevelStack.next->directoryLevel->name);
			printDirectory(headLevelStack.next->directoryLevel, 0);
			break;

		case '5': //end of program
			printf("\nEnd of program.\n");
			break;

		default:
			printf("Invalid input.\n");
			break;
		}
	} while (input != '5');

	deleteDirectory(&headDirectory);
	deleteStack(&headLevelStack);

	return 0;
}
	
//funkcija za stvaranje direktorija
DirectoryPosition createDirectory(char* directoryName) {

	DirectoryPosition newDirectory = NULL; //pokazivac na novi direktorij
	newDirectory = (DirectoryPosition)malloc(sizeof(Directory));
	if (newDirectory == NULL) return NULL;

	strcpy(newDirectory->name, directoryName); //kopira ime
	newDirectory->subdirectoryPosition = NULL; //nema poddirektorija
	newDirectory->next = NULL; //nema sljedeci direktorij
	return newDirectory;
}

//funkcija za stavljanje direktorija na vrh steka
int push(LevelStackPosition headLevelStack, DirectoryPosition currentDirectory) {

	LevelStackPosition newLevelStack = NULL;
	newLevelStack = (LevelStackPosition)malloc(sizeof(LevelStack));
	if (newLevelStack == NULL) return MALLOC_ERROR;
 
	newLevelStack->directoryLevel = currentDirectory; //postavlja direktorij na novi element steka
	newLevelStack->next = headLevelStack->next; //novi element pokazuje na onaj koji je prije head pokaziva, na prvi stari
	headLevelStack->next = newLevelStack; //head pokazuje na novi element

	return EXIT_SUCCESS;
}

//funkcija za brisanje direktorija sa vrha steka
int pop(LevelStackPosition headLevelStack) {

	if (headLevelStack->next == NULL ||
		headLevelStack->next->next == NULL)
		return EMPTY_STACK;

	LevelStackPosition temp; //pomocni pokazivac za element koji brisemo

	if (headLevelStack->next == NULL) return EMPTY_STACK; 

	temp = headLevelStack->next; 
	headLevelStack->next = headLevelStack->next->next;
	free(temp);
	return EXIT_SUCCESS;
}

//funkcija za dodavanje poddirektorija u direktorij
//superdirektorij-roditelj,subdirektorij-dijete koje dodajemo
int addDirectory(DirectoryPosition superdirectory, DirectoryPosition subdirectory) {
	subdirectory->next = superdirectory->subdirectoryPosition; //novo dijete pokazuje na staro prvo dijete
	superdirectory->subdirectoryPosition = subdirectory; //roditelj pokazuje na prvo dijete
	return EXIT_SUCCESS;
}

//funckija za mijenjanje direktorija na steku,prima pokazivac na stek i ime direktorija u koji zelimo ici
int changeDirectory(LevelStackPosition headLevelStack, char* directoryName) {
	DirectoryPosition current = headLevelStack->next->directoryLevel->subdirectoryPosition; //prvi pravi element steka je headLevelStack->next,subdirectoryPosition-pok na prvo dijete
	//prolazimo kroz sve poddirektorije trenutnog direktorija
	while (current != NULL && strcmp(current->name, directoryName) != 0)  current = current->next;
	if (current != NULL) { //nasli smo direktorij,jer jedino tada strcmp vraca 0
		printf("Changed directory to %s\n", directoryName);
		push(headLevelStack, current); //stavljamo nadeni direktorij na vrh steka
	}
	else printf("Directory not found.\n");
	return EXIT_SUCCESS;
}

//funkcija za ispis direkotrija i poddirektorija
int printDirectory(DirectoryPosition current, int depth) { //current-direktorij koji ispisujemo
	int i = 0;
	DirectoryPosition currentSubdirectory = NULL; //pokazivac koji ide kroz djecu direktorija
	if (current->subdirectoryPosition != NULL) { //prvo dijete
		currentSubdirectory = current->subdirectoryPosition;
		while (currentSubdirectory != NULL) {
			for (i = 0; i < depth; i++) printf("  "); //uvlacenje za bolji pregled
			printf("%s\n", currentSubdirectory->name);
			printDirectory(currentSubdirectory, depth + 1);
			currentSubdirectory = currentSubdirectory->next; //sljedece dijete
		}
	} return EXIT_SUCCESS;
}

//funkcija za brisanje direktorija i poddirektorija
int deleteDirectory(DirectoryPosition headDirectory) {
	DirectoryPosition temp = NULL; 
	while (headDirectory->next != NULL) {
		temp = headDirectory->next;
		headDirectory->next = headDirectory->next->next; 
		free(temp);
	}
	return EXIT_SUCCESS;
}

int deleteStack(LevelStackPosition headLevelStack) {
	LevelStackPosition temp = NULL;
	while (headLevelStack->next != NULL) {
		temp = headLevelStack->next;
		headLevelStack->next = headLevelStack->next->next;
		free(temp);
	}
	return EXIT_SUCCESS;
}



