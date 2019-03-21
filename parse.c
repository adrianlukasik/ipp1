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
#include "tree.h"

#define MAX_NUMBER_OF_DIGITS_ENERGY 20

Parameters getNewParameters() {
    Parameters p = malloc(sizeof(struct Parameters));
    p->energy = NULL;
    p->history_a = NULL;
    p->size_a = 0;
    p->history_b = NULL;
    p->size_b = 0;
    return p;
}

void removeParameters(Parameters p) {
    if (p != NULL) {
        if (p->energy != NULL)
            free(p->energy);
        if (p->history_a != NULL)
            free(p->history_a);
        if (p->history_b != NULL)
            free(p->history_b);
        free(p);
    }
}

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

int nextSpace(char *data, int i, int j) {
    for (int k = i; k <= j; k++)
        if (data[k] == ' ')
            return k;
    return 0;
}

int lastZero(char *data, int i, int j) {
    for (int k = i; k <= j; k++)
        if (data[k] != '0')
            return k - 1;
    return j;
}

void rightInstruction(Order order, Parameters p, Tree t) {
    switch (order) {
        case DECLARE:
            declareHistory(t, p->history_a, p->size_a);
            fprintf(stdout, "OK\n");
            break;
        case REMOVE:
            removeHistory(t, p->history_a, p->size_a);
            fprintf(stdout, "OK\n");
            break;
        case VALID:
            if (validHistory(t, p->history_a, p->size_a))
                fprintf(stdout, "YES\n");
            else
                fprintf(stdout, "NO\n");
            break;
        case ENERGY:
            if (p->energy == NULL)
                printEnergy(t, p->history_a, p->size_a);
            else
                changeEnergy(t, p->history_a, p->size_a, *(p->energy));
            break;
        case EQUAL:
            equalHistories(t, p->history_a, p->size_a, p->history_b, p->size_b);
            break;
        default:
            break;
    }
}

void copyString(char *data, int i, int j, Parameters p, int nr) {
    if (nr == 1) {
        p->size_a = j - i + 1;
        p->history_a = malloc(p->size_a * sizeof(char));
        for (int k = 0; k < p->size_a; k++)
            p->history_a[k] = data[i + k];
    } else if (nr == 2) {
        p->size_b = j - i + 1;
        p->history_b = malloc(p->size_b * sizeof(char));
        for (int k = 0; k < p->size_b; k++)
            p->history_b[k] = data[i + k];
    }
}

TYPE_OF_ENERGY stringToEnergy(char *data, int i, int j) {
    TYPE_OF_ENERGY energy = 0;
    for (int k = i; k <= j; k++) {
        energy *= 10;
        energy += (TYPE_OF_ENERGY)(data[k] - '0');
    }
    return energy;
}

void doRightInstruction(char *data, int i, Order order, Tree t, Parameters p) {
    if (isQuantumHistory(data, lenOrder(order), i - 1)) {
        copyString(data, lenOrder(order), i - 1, p, 1);
        rightInstruction(order, p , t);
    } else {
        incorrectLine();
    }
}

void makeInstruction(char *data, int i, Tree t) {
    int indexSpace, indexLastZero;
    TYPE_OF_ENERGY energy;
    Parameters parameters = getNewParameters();
    switch (whichOrder(data)) {
        case DECLARE:
            doRightInstruction(data, i, DECLARE, t, parameters);
            break;
        case REMOVE:
            doRightInstruction(data, i, REMOVE, t, parameters);
            break;
        case VALID:
            doRightInstruction(data, i, VALID, t, parameters);
            break;
        case ENERGY:
            indexSpace = nextSpace(data, lenOrder(ENERGY), i - 1);
            if (indexSpace != 0) {
                indexLastZero = lastZero(data, indexSpace + 1, i - 1);
                // czyli nie ma zer wiodących
                if (indexLastZero == indexSpace) {
                    if (isEnergy(data, indexSpace + 1, i - 1)) {
                        energy = stringToEnergy(data, indexSpace + 1, i - 1);
                        parameters->energy = malloc(sizeof(TYPE_OF_ENERGY));
                        *(parameters->energy) = energy;
                        doRightInstruction(data, indexSpace, ENERGY, t,
                                           parameters);
                    }
                    else {
                        incorrectLine();
                    }
                    // sprawdzamy czy dla liczb z przedziału [lenOrder(ENERGY), indexSpace - 1] mamy historie
                    // sprawdzamy czy dla liczb z przedziału [indexSpace + 1, i-1] mamy energie

                    // jesli tak to odpalamy funkcje w p.p. incorrectLine
                } else {
                    if (isEnergy(data, indexLastZero + 1, i - 1)) {
                        energy = stringToEnergy(data, indexLastZero + 1, i - 1);
                        parameters->energy = malloc(sizeof(TYPE_OF_ENERGY));
                        *(parameters->energy) = energy;
                        doRightInstruction(data, indexSpace, ENERGY, t,
                                           parameters);
                    } else {
                        incorrectLine();
                    }
                    // mamy zera wiodace
                    // sprawdzamy czy dla liczb z przedziału [lenOrder(ENERGY), indexSpace - 1] mamy historie
                    // sprawdzamy czy dla liczb z przedziału [indexLastZero + 1, i - 1] mamy energie
                }

            } else {
                doRightInstruction(data, i, ENERGY, t, parameters);
            }
            break;
        case EQUAL:
            indexSpace = nextSpace(data, lenOrder(EQUAL), i - 1);
            if (indexSpace != 0) {
                if (isQuantumHistory(data, indexSpace + 1, i - 1)) {
                    copyString(data, indexSpace + 1, i - 1, parameters, 2);
                    doRightInstruction(data, indexSpace, EQUAL, t, parameters);
                }
                else {
                    incorrectLine();
                }
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
    removeParameters(parameters);
}
