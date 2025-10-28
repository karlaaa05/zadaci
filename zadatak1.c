#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_POINTS 100
#define MAX_LINE 100
#define FILE_NOT_OPENED -1
#define MEMORY_ERROR -2

//struktura koja predstavlja studenta
struct _Student;
typedef struct _Student {
	char name[50];
	char surname[50];
	int points;
	struct _Student* next;
} Student;

//deklaracija funkcija
int num_of_students(char* filename);
Student* load_students(char* filename, int num_students);
float* calculate_relative(Student* students, int num_students, int max_points);
int print_students(Student* students, float* rel_points, int num_students);


int main() {
	char filename[] = "studenti.txt";
	int num_students = num_of_students(filename);
	if (num_students == FILE_NOT_OPENED) {
		printf("Error opening file.\n");
		return FILE_NOT_OPENED;
	}
	Student* students = load_students(filename, num_students);
	if (students == NULL) {
		printf("Memory allocation error.\n");
		return MEMORY_ERROR;
	}
	float max_points = 100.0;
	float* rel_points = calculate_relative(students, num_students, max_points);
	if (rel_points == NULL) {
		free(students);
		return MEMORY_ERROR;
	}

	print_students(students, rel_points, num_students);

	free(rel_points);   
	free(students);
	return 0;
}
//funkcija koja broji broj studenata u datoteci

int num_of_students(char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Datoteka se ne moze otvoriti.\n");
		return FILE_NOT_OPENED;
	}
	int count = 0;
	char line[MAX_LINE];
	while (fgets(line, sizeof(line), file) != NULL) {
		count++;
	}
	fclose(file);
	return count;
}
//funkcija koja alocira memoriju i ucitava sve podatke o studentu iz datoteke

Student* load_students(char* filename, int num_students) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Datoteka se ne moze otvoriti.\n");
		return NULL;
	}
	//provjerava je li ispravan broj redaka 
	if(num_students <= 0 ) {
		printf("Neispravan broj studenata.\n");
		fclose(file);
		return NULL;
	}
	//alokacija memorije za studente i provjera alokacije
	Student* students = (Student*)malloc(num_students * sizeof(Student));
	if(students == NULL) {
		printf("Greska pri alokaciji memorije.\n");
		fclose(file);
		return NULL;
	}

	//ide kroz datoteku i ucitava podatke o studentima, ako ima manje od 3 podataka po studentu, ispisuje gresku
	for(int i=0; i < num_students; i++) {
		if(fscanf(file, "%s %s %d", students[i].name, students[i].surname, &students[i].points) != 3) {
			printf("Greska pri citanju podataka o studentu.\n");
			free(students);
			fclose(file);
			return NULL;  }
	}
	fclose(file);
    return students;

}
//funkcija koja izracunava max broj bodova i alocira memoriju za relativne bodove, u for petlji izracunava relativne bodove i vraca ih
float* calculate_relative(Student* students, int num_students, int max_points) {

	//alokacija memorije za relativne bodove
	float* rel_points = (float*)malloc(num_students * sizeof(float));
	if (rel_points == NULL) {
		printf("Greska pri alokaciji memorije za relativne bodove.\n");
		return NULL;
	}
	//izracunavanje relativnih bodova
	for (int i = 0; i < num_students; i++) {
		rel_points[i] = ((float)students[i].points / max_points) * 100.0;
	}
	return rel_points;
}
    //funkcija koja ispisuje sve podatke o studentima 
	int print_students(Student* students, float* rel_points, int num_students) {
		printf("Podaci o studentima:\n");
		printf("Ime\tPrezime\tBodovi\tRelativni bodovi\n");
		for (int i = 0; i < num_students; i++) {
			printf("%s\t%s\t%d\t%.2f\n", students[i].name, students[i].surname, students[i].points, rel_points[i]);
		}

		return 0;
	}
		