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

Error ListAddBeforeElement(List *const list, const ListElemData_t element, const unsigned count);

Error ListAddAfterElement(List *const list, const ListElemData_t element, const unsigned count);

Error ListAddBeforeHead(List *const list, const ListElemData_t element);

Error ListAddAfterTail(List *const list, const ListElemData_t element);

Error ListRemoveElement(List *const list, const unsigned count);

void ListDump(const List *const lst, const char *name, const char *fileCalledAt, const char *funcCalledAt,
              int lineCalledAt);

ListElement *ListGetElementByCount(List *const list, const unsigned count);

int ListGetIndexOfElement(const List *const list, const ListElement *const element);

unsigned ListGetElementCountByIndex(const List *const list, const int index);

#endif
