#include <stdlib.h>
#include "list.h"

List getNewLnode() {
    List node = malloc(sizeof(struct Lnode));
    node->next = NULL;
    node->prev = NULL;
    node->root = NULL;
    return node;
}

Root getNewLroot(List l, TYPE_OF_ENERGY energy) {
    Root root = malloc(sizeof(struct Lroot));
    root->begin = root->end = l;
    root->energy = energy;
    root->counter = 1;
    return root;
}

void addNewElement(Root r, List l) {
    List list = r->end;
    list->next = l;
    l->prev = list;
    l->root = r;
    r->end = l;
    (r->counter)++;
}

void changeRoot(List l, Root r) {
    while (l != NULL) {
        l->root = r;
        l = l->next;
    }
}

void switchRoot(Root rootRemoved, Root rootAdded) {
    rootAdded->energy = (rootAdded->energy + rootRemoved->energy) / 2;
    rootAdded->counter += rootRemoved->counter;
    changeRoot(rootRemoved->begin, rootAdded);
    List first = rootRemoved->begin;
    List last = rootAdded->end;
    last->next = first;
    first->prev = last;
    rootAdded->end = rootRemoved->end;
    free(rootRemoved);
}

void switchList(List l) {
    if (l != NULL) {
        List left = l->prev;
        List right = l->next;
        if (left != NULL)
            left->next = right;
        if (right != NULL)
            right->prev = left;
    }
}

void modifyRoot(List l) {
    if (l != NULL) {
        Root root = l->root;
        if (root->begin == l)
            root->begin = l->next;
        if (root->end == l)
            root->end = l->prev;
        (root->counter)--;
    }
}

void clearLnode(List l) {
    if (l != NULL) {
        switchList(l);
        modifyRoot(l);
        if (l->root->counter == 0)
            free(l->root);
        free(l);
    }
}
