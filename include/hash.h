#ifndef HASH_H
#define HASH_H

typedef struct charList* CharpList;

#define MIN_C(0)
#define MAX_C(128)

// Makes hashtable out of unsorted list of n chars
// Returns 0 if allocation fails
CharpList CharpListCreate(int n, int unsortedCharList[]);

void CharpListDestroy(CharpList list);

int CharpListContains(CharpList list, int charp);

#endif