#include <stdbool.h>
#include "tree.h"

#ifndef MALE_ZADANIE1_PARSE_H
#define MALE_ZADANIE1_PARSE_H

struct Parameters;

typedef struct Parameters* Parameters;

struct Parameters {
    char *history_a;
    int size_a;
    char *history_b;
    int size_b;
    TYPE_OF_ENERGY *energy;
};

/* Zwraca wskaźnik do nowego elementu typu struct Parameters. */
Parameters getNewParameters();

/* Kasuje obiekt wskazywany przez p. */
void removeParameters(Parameters p);

typedef enum Order {
    DECLARE, REMOVE, VALID, ENERGY, EQUAL, TOTAL
} Order;

/* Zwraca nazwę polecenia ze spacją lub pusty napis. */
char* orderToString(Order order);

/* Zwraca polecenie, które znajduje się w tablicy data. */
Order whichOrder(char *data);

/* Sprawdza czy znak c jest stanem kwantowym. */
bool isQuantumState(char c);

/* Sprawdza czy napis data[i...j] jest kwantową historią. */
bool isQuantumHistory(char *data, int i, int j);

/* Sprawdza czy napis data[i...j] jest liczbą. */
bool isNumber(char *data, int i, int j);

/* Sprawdza czy napis data[i...j] jest kandydatem do bycia energią, czyli czy
 * powyższe słowo jest leksykograficznie mniejsze od stałej ULLONG_MAX. */
bool isCandidateToEnergy(char *data, int i, int j);

/* Sprawdza czy napis data[i...j] jest energią. */
bool isEnergy(char *data, int i, int j);

/* Wypisuje informację o nieprawidłowej linii na stderr. */
void incorrectLine();

/* Wczytuje linię do momentu napotkania znaku końca linii. */
void ignoreLine();

/* Wczytuje dane do tablicy data. */
void loadLine(char **data, int *i, int *size);

/* Zwraca długość nazwy polecenia z uwzględnioną spacją. */
int lenOrder(Order order);

/* Zwraca wartość indeksu podtablicy data[i...j], w której znajduje się
 * najbliższa spacja, jeśli taki indeks nie istnieje zwraca 0. */
int nextSpace(char *data, int i, int j);

/* Zwraca pozycję ostatniego zera w podtablicy data[i...j]. */
int lastZero(char *data, int i, int j);

/* Wykonuje polecenie z danymi podanymi w data[0...i]. */
void makeInstruction(char *data, int i, Tree t);

/* Kopiuje napis data[i...j] do elementu history_nr wskazywanego przez p. */
void copyString(char *data, int i, int j, Parameters p, int nr);

/* Zwraca wartość energii przechowywanej w napisie data[i...j]. */
TYPE_OF_ENERGY stringToEnergy(char *data, int i, int j);

#endif //MALE_ZADANIE1_PARSE_H
