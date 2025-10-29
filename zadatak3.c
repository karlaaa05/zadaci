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


Person* Ispisliste(Person* head)
{
    Person* current = head;
    if (current == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }
    while (current != NULL)
    {
        printf("%s %s %d\n", current->name, current->surname, current->age);
        current = current->next;
    }
    return head;
}

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

Person* DodajNaPocetak(Person* head)
{
    Person* novaosoba = UnosElementa();
    if (!novaosoba) return head;
    novaosoba->next = head;
    return novaosoba;
}

Person* DodajNaKraj(Person* head)
{
    Person* novaosoba = UnosElementa();
    if (!novaosoba) return head;

    if (head == NULL) return novaosoba;

    Person* current = head;
    while (current->next != NULL) current = current->next;
    current->next = novaosoba;
    return head;
}

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
            return head;
        }
        current = current->next;
    }
    printf("Clan nije pronaden na listi.\n");
    return head;
}

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

// novi dio - funckije
// dodaj novi element iza nekog prezimena
Person* DodajIzaElementa(Person* head)
{
    char targetSurname[NAMESIZE];
    printf("Unesi prezime osobe iza koje zelis dodati novu osobu: ");
    scanf_s("%49s", targetSurname, (unsigned)_countof(targetSurname));

    Person* current = head;
    while (current != NULL && strcmp(current->surname, targetSurname) != 0)
        current = current->next;

    if (current == NULL) {
        printf("Osoba s prezimenom %s nije pronadena.\n", targetSurname);
        return head;
    }

    Person* nova = UnosElementa();
    if (!nova) return head;

    nova->next = current->next;
    current->next = nova;

    printf("Nova osoba dodana iza %s.\n", targetSurname);
    return head;
}

// dodaj novi element ispred nekog prezimena
Person* DodajIspredElementa(Person* head)
{
    char targetSurname[NAMESIZE];
    printf("Unesi prezime osobe ispred koje zelis dodati novu osobu: ");
    scanf_s("%49s", targetSurname, (unsigned)_countof(targetSurname));

    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }

    // ako treba dodati ispred prvog elementa
    if (strcmp(head->surname, targetSurname) == 0)
    {
        Person* nova = UnosElementa();
        if (!nova) return head;
        nova->next = head;
        return nova;
    }

    Person* current = head;
    while (current->next != NULL && strcmp(current->next->surname, targetSurname) != 0)
        current = current->next;

    if (current->next == NULL) {
        printf("Osoba s prezimenom %s nije pronadena.\n", targetSurname);
        return head;
    }

    Person* nova = UnosElementa();
    if (!nova) return head;

    nova->next = current->next;
    current->next = nova;

    printf("Nova osoba dodana ispred %s.\n", targetSurname);
    return head;
}

// sortiranje liste po prezimenima (bubble sort)
Person* SortirajPoPrezimenu(Person* head)
{
    if (head == NULL || head->next == NULL)
        return head;

    int swapped;
    Person* ptr1;
    Person* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->surname, ptr1->next->surname) > 0)
            {
                // Zamjena podataka
                char tempName[NAMESIZE], tempSurname[NAMESIZE];
                int tempAge;

                strcpy(tempName, ptr1->name);
                strcpy(tempSurname, ptr1->surname);
                tempAge = ptr1->age;

                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->surname, ptr1->next->surname);
                ptr1->age = ptr1->next->age;

                strcpy(ptr1->next->name, tempName);
                strcpy(ptr1->next->surname, tempSurname);
                ptr1->next->age = tempAge;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("Lista je sortirana po prezimenima.\n");
    return head;
}

// upis liste u datoteku
void UpisiUDatoteku(Person* head)
{
    FILE* fp;
    fopen_s(&fp, "lista.txt", "w");
    if (!fp) {
        printf("Greska pri otvaranju datoteke.\n");
        return;
    }

    Person* current = head;
    while (current != NULL)
    {
        fprintf(fp, "%s %s %d\n", current->name, current->surname, current->age);
        current = current->next;
    }

    fclose(fp);
    printf("Lista uspjesno upisana u datoteku lista.txt\n");
}

// citanje liste iz datoteke
Person* CitajIzDatoteke(Person* head)
{
    FILE* fp;
    fopen_s(&fp, "lista.txt", "r");
    if (!fp) {
        printf("Greska pri otvaranju datoteke.\n");
        return head;
    }

    head = IzbrisiListu(head); // isprazni staru listu
    Person* last = NULL;

    while (!feof(fp))
    {
        Person* nova = (Person*)malloc(sizeof(Person));
        if (fscanf_s(fp, "%49s %49s %d", nova->name, (unsigned)_countof(nova->name),
            nova->surname, (unsigned)_countof(nova->surname), &nova->age) == 3)
        {
            nova->next = NULL;
            if (head == NULL) head = nova;
            else last->next = nova;
            last = nova;
        }
        else {
            free(nova);
            break;
        }
    }

    fclose(fp);
    printf("Lista ucitana iz datoteke.\n");
    return head;
}


int main()
{
    int odabir = 0;
    Person* head = NULL;

    do
    {
        printf("\nOdaberi radnju:\n");
        printf("1. Ispisi listu\n2. Dodaj na pocetak\n3. Dodaj na kraj\n4. Pronadi po prezimenu\n");
        printf("5. Dodaj iza elementa\n6. Dodaj ispred elementa\n7. Sortiraj po prezimenu\n");
        printf("8. Upisi u datoteku\n9. Citaj iz datoteke\n10. Izbrisi listu\n0. Izlaz\n");
        scanf_s("%d", &odabir);

        switch (odabir)
        {
        case 1: head = Ispisliste(head); break;
        case 2: head = DodajNaPocetak(head); break;
        case 3: head = DodajNaKraj(head); break;
        case 4: head = PronadiPoPrezimenu(head); break;
        case 5: head = DodajIzaElementa(head); break;
        case 6: head = DodajIspredElementa(head); break;
        case 7: head = SortirajPoPrezimenu(head); break;
        case 8: UpisiUDatoteku(head); break;
        case 9: head = CitajIzDatoteke(head);
            head = Ispisliste(head);
            break;
        case 10: head = IzbrisiListu(head); break;
        case 0: break;
        default: printf("Nepoznata opcija!\n"); break;
        }

    } while (odabir != 0);

    head = IzbrisiListu(head);
    return 0;
}
