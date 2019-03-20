//
// Created by root on 16.03.19.
//

#include <stdbool.h>
#include <stdio.h>
#include "tree.h"

int digitToInt(char c) {
    return c - '0';
}

Tree getNewTnode() {
    Tree node = malloc(sizeof(struct Tnode));
    for (int i = 0; i < NUMBER_OF_QUANTUM_STATES; i++)
        node->state[i] = NULL;
    node->history = NULL;
    return node;
}

void clearData(Tree t) {
    if (t != NULL) {
        clearLnode(t->history);
        for (int i = 0; i < NUMBER_OF_QUANTUM_STATES; i++)
            clearData(t->state[i]);
        free(t);
    }
}

void declareHistory(Tree t, char *history, int n) {
    for (int i = 0; i < n; i++) {
        if (t->state[digitToInt(history[i])] == NULL)
            t->state[digitToInt(history[i])] = getNewTnode();
        t = t->state[digitToInt(history[i])];
    }
}

Tree* firstToRemove(Tree t, char *history, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (t->state[digitToInt(history[i])] == NULL)
            return NULL;
        t = t->state[digitToInt(history[i])];
    }
    return &(t->state[digitToInt(history[n - 1])]);
}

void removeHistory(Tree t, char *history, int n) {
    Tree *tree = firstToRemove(t, history, n);
    clearData(*tree);
    *tree = NULL;
}

Tree findTnode(Tree t, char *history, int n) {
    for (int i = 0; i < n; i++) {
        if (t->state[digitToInt(history[i])] == NULL)
            return NULL;
        t = t->state[digitToInt(history[i])];
    }
    return t;
}

bool validHistory(Tree t, char *history, int n) {
    return findTnode(t, history, n) != NULL;
}

void printEnergy(Tree t, char *history, int n) {
    Tree tree = findTnode(t, history, n);
    if (tree != NULL) {
        List list = tree->history;
        if (list != NULL) {
            Root root = list->root;
            if (root != NULL)
                printf("%llu\n", root->energy);
        }
    }
}

void changeEnergy(Tree t, char *history, int n, TYPE_OF_ENERGY energy) {
    Tree tree = findTnode(t, history, n);
    if (tree != NULL) {
        List list = tree->history;
        if (list == NULL) {
            tree->history = getNewLnode();
            list = tree->history;
            list->root = getNewLroot(list, energy);
        } else {
            list->root->energy = energy;
        }
    }
}

// warto sprawdzic czy list maja tego samego roota
void equalHistories(Tree t, char *history_a, int n, char *history_b, int m) {
    Tree tree_a = findTnode(t, history_a, n);
    Tree tree_b = findTnode(t, history_b, m);
    if (tree_a != NULL && tree_b != NULL) {
        if (tree_a != tree_b) {
            List l_a = tree_a->history;
            List l_b = tree_b->history;
            if (l_a != NULL || l_b != NULL) {
                if (l_a == NULL) {
                    tree_a->history = getNewLnode();
                    addNewElement(l_b->root, tree_a->history);
                } else if (l_b == NULL) {
                    tree_b->history = getNewLnode();
                    addNewElement(l_a->root, tree_b->history);
                } else if (l_a->root != l_b->root){
                    // oba nie sa nullami zatem trzeba zrobic przepinanie
                    int counter_a = l_a->root->counter;
                    int counter_b = l_b->root->counter;
                    if (counter_a > counter_b) {
                        // przepinamy liste b do listy a
                        switchRoot(l_b->root, l_a->root);
                    } else {
                        // przepinamy liste a do listy b
                        switchRoot(l_a->root, l_b->root);
                    }
                }
            }
        }
    }
}