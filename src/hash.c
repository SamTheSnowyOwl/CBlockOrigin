#include <stdlib.h>
#include <assert.h>

#include "hash.h"

#define OVERHEAD (1.1)
#define NULL_CHARP (-1)

struct charList {
    int size;
    int charp[1];
};

CharpList CharpListCreate(int n, int unsortedCharList[]) {
    CharpList list;
    int size;
    int probe;

    size = (int)(n*OVERHEAD+1);

    list = malloc(sizeof(*list) + sizeof(int) * (size-1));
    if(list == 0) return 0;

    list->size = size;

    for(int i = 0; i < size; i++) {
        list->charp[i] = NULL_CHARP;
    }

    for(int i = 0; i < size; i++) {
        assert(unsortedCharList[i] >= MIN_ID);
        assert(unsortedCharList[i] <= MAX_ID);

        for(probe = unsortedCharList[i] % list->size;
            list->charp[probe] != NULL_CHARP;
            probe = (probe+1) % list->size);

        assert(list->charp[probe] == NULL_CHARP);

        list->charp[probe] = unsortedCharList[i];
    }

    return list;
}

void CharpListDestroy(CharpList list) {
    free(list);
}

int CharpListContains(CharpList list, int charp) {
    int probe;

    for(probe = charp %)
}