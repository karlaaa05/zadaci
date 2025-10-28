#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 50

typedef struct Person
{
    char name[NAMESIZE];
    char surname[NAMESIZE];
    int age;
    struct Person* next;
} Person;

// Funkcija za ispis liste, vraæa head
Person* Ispisliste(Person* head)
{
    Person* current = head;
    if (current == NULL) {
        printf("Lista je prazna.\n");
    }
    while (current != NULL)
    {
        printf("%s %s %d\n", current->name, current->surname, current->age);
        current = current->next;
    }
    return head;
}

// Funkcija za unos novog elementa, vraæa pokazivaè na novi element
Person* UnosElementa()
{
    Person* novaosoba = (Person*)malloc(sizeof(Person));
    if (novaosoba == NULL) return NULL;

    printf("Unesi ime: ");
    scanf_s("%49s", novaosoba->name, (unsigned)_countof(novaosoba->name));
    printf("Unesi prezime: ");
    scanf_s("%49s", novaosoba->surname, (unsigned)_countof(novaosoba->surname));
    printf("Unesi godine: ");
    scanf_s("%d", &novaosoba->age);
    novaosoba->next = NULL;
    return novaosoba;
}

// Dodavanje na poèetak liste
Person* DodajNaPocetak(Person* head)
{
    Person* novaosoba = UnosElementa();
    novaosoba->next = head;
    head = novaosoba;
    return head;
}

// Dodavanje na kraj liste
Person* DodajNaKraj(Person* head)
{
    Person* novaosoba = UnosElementa();
    if (head == NULL) return novaosoba;

    Person* current = head;
    while (current->next != NULL) current = current->next;
    current->next = novaosoba;
    return head;
}

// Pronalazak elementa po prezimenu, vraæa head
Person* PronadiPoPrezimenu(Person* head)
{
    char wantedSurname[NAMESIZE];
    printf("Unesi prezime clana za pretragu: ");
    scanf_s("%49s", wantedSurname, (unsigned)_countof(wantedSurname));

    Person* current = head;
    while (current != NULL)
    {
        if (strcmp(current->surname, wantedSurname) == 0)
        {
            printf("Clan pronaden: %s %s %d\n", current->name, current->surname, current->age);
            return head; // ne mijenja listu
        }
        current = current->next;
    }
    printf("Clan nije pronaden na listi.\n");
    return head;
}

// Brisanje cijele liste
Person* IzbrisiListu(Person* head)
{
    Person* current = head;
    while (current != NULL)
    {
        Person* temp = current;
        current = current->next;
        free(temp);
    }
    return NULL;
}

int main()
{
    int odabir = 0;
    Person* head = NULL;

    do
    {
        printf("\nOdaberi radnju:\n");
        printf("1. Ispisi listu\n2. Dodaj element na pocetak\n3. Dodaj element na kraj\n4. Pronadi po prezimenu\n5. Izbrisi listu\n0. Izlaz\n");
        scanf_s("%d", &odabir);

        switch (odabir)
        {
        case 1: head = Ispisliste(head); break;
        case 2: head = DodajNaPocetak(head); break;
        case 3: head = DodajNaKraj(head); break;
        case 4: head = PronadiPoPrezimenu(head); break;
        case 5: head = IzbrisiListu(head); break;
        case 0: break;
        default: printf("Nepoznata opcija!\n"); break;
        }

    } while (odabir != 0);

    // prije izlaza osiguraj da je lista prazna
    head = IzbrisiListu(head);
    return 0;
}
