#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "parse.h"

#define SIZE_OF_DATA 8
#define MIN_LENGTH_CORRECT_LINE 7

int main() {
    int i = 1, size = SIZE_OF_DATA;
    char *data = malloc(size * sizeof(char));
    Tree tree = getNewTnode();
    while ((data[0] = getchar()) != EOF) {
        if (data[0] == '\n')
            continue;
        if (data[0] == '#') {
            ignoreLine();
            continue;
        }
        loadLine(&data, &i, &size);
        if (i < MIN_LENGTH_CORRECT_LINE)
            incorrectLine();
        else
            makeInstruction(data, i);
        size = SIZE_OF_DATA;
        data = realloc(data, size * sizeof(char));
        assert(data);
        i = 1;
    }
    clearData(tree);
    free(data);
    return 0;
}