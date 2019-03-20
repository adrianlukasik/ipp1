//
// Created by root on 14.03.19.
//

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include "parse.h"

#define MAX_NUMBER_OF_DIGITS_ENERGY 20

char* orderToString(Order order) {
    switch (order) {
        case DECLARE:
            return "DECLARE ";
        case REMOVE:
            return "REMOVE ";
        case VALID:
            return "VALID ";
        case ENERGY:
            return "ENERGY ";
        case EQUAL:
            return "EQUAL ";
        default:
            return "";
    }
}

Order whichOrder(char *data) {
    char *c;
    for (Order order = DECLARE; order < TOTAL; order++) {
        c = orderToString(order);
        if (strncmp(c, data, strlen(c)) == 0) return order;
    }
    return TOTAL;
}

bool isQuantumState(char c) {
    return c == '0' || c == '1' || c == '2' || c == '3';
}

bool isQuantumHistory(char *data, int i, int j) {
    if (i > j) return false;
    for (int k = i; k <= j; k++)
        if (!isQuantumState(data[k]))
            return false;
    return true;
}

bool isNumber(char *data, int i, int j) {
    if (i > j) return false;
    for (int k = i; k <= j; k++)
        if (!isdigit(data[k]))
            return false;
    return true;
}

bool isCandidateToEnergy(char *data, int i, int j) {
    if (i > j) return false;
    char tmp[j - i + 1];
    for (int k = 0; k < j - i + 1; k++)
        tmp[k] = data[k + i];
    char maxULongLong[MAX_NUMBER_OF_DIGITS_ENERGY];
    sprintf(maxULongLong, "%llu", ULLONG_MAX);
    return strcmp(maxULongLong, tmp) >= 0;
}

bool isEnergy(char *data, int i, int j) {
    if (i > j) return false;
    if ((j - i + 1) > MAX_NUMBER_OF_DIGITS_ENERGY)
        return false;
    else {
        bool b = true;
        if (j - i + 1 == MAX_NUMBER_OF_DIGITS_ENERGY)
            b = isCandidateToEnergy(data, i, j);
        return b && isNumber(data, i, j);
    }
}

void incorrectLine() {
    fprintf(stderr, "ERROR\n");
}

void ignoreLine() {
    while (getchar() != '\n') {}
}

void loadLine(char **data, int *i, int *size) {
    while (((*data)[*i] = getchar()) != '\n') {
        ++(*i);
        if (*i == *size) {
            *size *= 2;
            *data = realloc(*data, *size * sizeof(char));
            assert(*data);
        }
    }
}

int lenOrder(Order order) {
    return strlen(orderToString(order));
}

void doTestowania(int i) {
    printf("do testowania %d\n", i);
}

/* Zwraca wartość indeksu tablicy data, w której znajduje się najbliższa spacja,
 * jeśli taki indeks nie istnieje zwraca 0. */
int nextSpace(char *data, int i, int j) {
    for (int k = i; k <= j; k++)
        if (data[k] == ' ')
            return k;
    return 0;
}

/* Zwraca pozycję ostatniego zera w tablicy data. */
int lastZero(char *data, int i, int j) {
    for (int k = i; k <= j; k++)
        if (data[k] != '0')
            return k - 1;
    return j;
}

void rightInstruction(Order order) {
    switch (order) {
        case DECLARE:
            printf("DECLARE!\n");
            break;
        case REMOVE:
            printf("REMOVE!\n");
            break;
        case VALID:
            printf("VALID!\n");
            break;
        case ENERGY:
            printf("ENERGY!\n");
            break;
        case EQUAL:
            printf("EQUAL!\n");
            break;
        default:
            break;
    }
}

void doRightInstruction(char *data, int i, Order order) {
    if (isQuantumHistory(data, lenOrder(order), i - 1))
        // todo utworz strukture w ktorej przechowujemy dane do wykoniania
        rightInstruction(order);
    else
        incorrectLine();
}
//todo tutaj skonczylem
void makeInstruction(char *data, int i) {
    int indexSpace, indexLastZero;
    /* najpierw sprawdzam 5-8 pierwszych znakow czy tworza
     * nazwe jakiegos polecenia ze spacją. */
    switch (whichOrder(data)) {
        case DECLARE:
            doRightInstruction(data, i, DECLARE);
            break;
        case REMOVE:
            doRightInstruction(data, i, REMOVE);
            break;
        case VALID:
            doRightInstruction(data, i, VALID);
            break;
        case ENERGY:
            indexSpace = nextSpace(data, lenOrder(ENERGY), i - 1);
            if (indexSpace != 0) {
                indexLastZero = lastZero(data, indexSpace + 1, i - 1);
                doTestowania(indexSpace);
                doTestowania(indexLastZero);
                // czyli nie ma zer wiodących
                if (indexLastZero == indexSpace) {
                    if (isEnergy(data, indexSpace + 1, i - 1))
                        doRightInstruction(data, indexSpace, ENERGY);
                    else
                        incorrectLine();
                    // sprawdzamy czy dla liczb z przedziału [lenOrder(ENERGY), indexSpace - 1] mamy historie
                    // sprawdzamy czy dla liczb z przedziału [indexSpace + 1, i-1] mamy energie

                    // jesli tak to odpalamy funkcje w p.p. incorrectLine
                } else {
                    if (isEnergy(data, indexLastZero + 1, i - 1))
                        doRightInstruction(data, indexSpace, ENERGY);
                    else
                        incorrectLine();
                    // mamy zera wiodace
                    // sprawdzamy czy dla liczb z przedziału [lenOrder(ENERGY), indexSpace - 1] mamy historie
                    // sprawdzamy czy dla liczb z przedziału [indexLastZero + 1, i - 1] mamy energie
                }

            } else {
                doRightInstruction(data, i, ENERGY);
            }
            break;
        case EQUAL:
            indexSpace = nextSpace(data, lenOrder(EQUAL), i - 1);
            if (indexSpace != 0) {
                // sprawdzamy czy dla liczb z przedziału [lenOrder(EQUAL) + 1, indexSpace - 1] mamy historie
                // sprawdzamy czy dla liczb z przedziału [indexSpace + 1, i - 1] mamy historie
            } else {
                incorrectLine();
            }
            break;
        default:
            incorrectLine();
            break;
    }
}
