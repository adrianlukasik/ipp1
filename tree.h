//
// Created by root on 16.03.19.
//

#ifndef MALE_ZADANIE1_TREE_H
#define MALE_ZADANIE1_TREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

#define NUMBER_OF_QUANTUM_STATES 4

struct Tnode;

typedef struct Tnode* Tree;

struct Tnode {
    Tree state[NUMBER_OF_QUANTUM_STATES];
    List history;
};

int digitToInt(char c);

/* Tworzy nowy Tnode i zwraca wskaźnik do niego. */
Tree getNewTnode();

/* Zwalnia strukturę danych. */
void clearData(Tree t);

/* Znajduje w drzewie t wskaźnik na Tnode związany z history. */
Tree findTnode(Tree t, char *history, int n);

/* Zwraca adres wskaźnika do pierwszego elementu, który chcemy usunąć. */
Tree* firstToRemove(Tree t, char *history, int n);

/* Dodaje każdy prefiks history do drzewa t. */
void declareHistory(Tree t, char *history, int n);

/* Usuwa wszystkie kwantowe historie z drzewa t,
 * które zawierają prefiks history. */
void removeHistory(Tree t, char *history, int n);

/* Zwraca czy drzewo t zawiera history. */
bool validHistory(Tree t, char *history, int n);

/* Jeśli history jest dopuszczalna, to przypisuje energy gdy history nie ma
 * przypisanej energii lub modyfikuje wartość energii w przeciwnym przypadku. */
void changeEnergy(Tree t, char *history, int n, TYPE_OF_ENERGY energy);

/* Jeśli history_a i history_b są dopuszczalne, to zrównuje ich energie. */
void equalHistories(Tree t, char *history_a, int n, char *history_b, int m);

/* Jeśli history jest dopuszczalna i ma przypisaną energię,
 * to wypisuje wartość tej energii. */
void printEnergy(Tree t, char *history, int n);

/* Zmienia znak, który jest cyfrą na liczbę. */
int digitToInt(char c);

#endif //MALE_ZADANIE1_TREE_H
