#ifndef LIST_H

#define LIST_H

#include "errorHandle.h"
#include <cstdint>

typedef int ListElemData_t;

const uint64_t cPoisonValue = 0xDED505Al;
const int cDefaultIndexValue = 0;
const ListElemData_t cZeroElementData = 316;

struct ListElement
{
    ListElemData_t data;
    int prevIndex;
    int nextIndex;
};

struct List
{
    ListElement *data;
    int *freeIndexes;
    unsigned size;
};

Error ListInit(List *const list, const size_t initSize);

void ListFree(List *const list);

Error ListAddElement(List *const list, const ListElemData_t element, const unsigned number);

Error ListRemoveElement(List *const list, const unsigned number);

void ListDump(const List *const lst, const char *name, const char *fileCalledAt, const char *funcCalledAt,
              int lineCalledAt);

ListElement *ListGetElementByNumber(List *const list, const unsigned number);

int ListGetIndexOfElement(const List *const list, const ListElement *const element);

#endif
