#ifndef MALE_ZADANIE1_LIST_H
#define MALE_ZADANIE1_LIST_H

typedef unsigned long long TYPE_OF_ENERGY;

struct Lroot;
struct Lnode;

typedef struct Lroot* Root;
typedef struct Lnode* List;

struct Lnode {
    List next;
    List prev;
    Root root;
};

struct Lroot {
    List begin;
    List end;
    TYPE_OF_ENERGY energy;
    int counter;
};

/* Tworzy nowy Lnode i zwraca wskaźnik do niego. */
List getNewLnode();

/* Tworzy nowy Lroot i zwraca wskaźnik do niego. */
Root getNewLroot(List l, TYPE_OF_ENERGY energy);

/* Dodaje l na koniec listy wskazywanej przez r. */
void addNewElement(Root r, List l);

/* Zmienia korzeń wszystkich elementów listy wskazywanej przez l. */
void changeRoot(List l, Root r);

/* Przepina listę wskazywaną przez rootRemoved do listy wskazywanej przez
 * rootAdded. */
void switchRoot(Root rootRemoved, Root rootAdded);

/* Łączy sąsiadów l ze sobą. */
void switchList(List l);

/* Modyfikuje root uwzględniając usunięcie listy l. */
void modifyRoot(List l);

/* Usuwa Lnode wskazywanego przez listę l. Przepina listę dwukierunkową oraz
 * dokonuje modyfikacji w root (w tym usunięcie root, gdy lista dwukierunkowa
 * jest pusta. */
void clearLnode(List l);

#endif //MALE_ZADANIE1_LIST_H
