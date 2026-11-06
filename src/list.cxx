#include "h/list.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Error ListInit(List *const list, const size_t initSize)
{
    ASSERT(list);

    list->data = (ListElement *)calloc(initSize, sizeof(ListElement));
    ALLOCATION_CHECK(list->data);

    list->freeIndexes = (int *)calloc(initSize, sizeof(int));
    ALLOCATION_CHECK(list->freeIndexes);

    for (size_t i = 0; i < initSize; ++i)
    {
        list->data[i].data = cPoisonValue;
        list->data[i].prevIndex = cDefaultIndexValue;
        list->data[i].nextIndex = cDefaultIndexValue;
    }

    list->data[0].data = cZeroElementData;

    for (size_t i = 1; i < initSize; ++i)
    {
        list->freeIndexes[i - 1] = i;
    }

    list->size = initSize;

    return CreateSuccess();
}

void ListFree(List *const list)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    free(list->data);
    free(list->freeIndexes);

    list->size = 0;

    return;
}

Error ListAddBeforeElement(List *const list, const ListElemData_t element, const unsigned count)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    int prevElementIndex = ListGetElementByCount(list, count)->prevIndex;
    int nextOfPrevElementIndex = list->data[prevElementIndex].nextIndex;

    if (list->freeIndexes[0] <= 0)
    {
        return CreateError(ecNoFreeIndexesLeft, "");
    }
    int indexToPlace = list->freeIndexes[0];

    // clang-format off
    list->data[indexToPlace] =
    {
    	.data = element,
     	.prevIndex = prevElementIndex,
      	.nextIndex = nextOfPrevElementIndex
    };
    // clang-format on

    list->data[prevElementIndex].nextIndex = indexToPlace;
    list->data[nextOfPrevElementIndex].prevIndex = indexToPlace;

    for (int i = 1; i < list->size; i++)
    {
        list->freeIndexes[i - 1] = list->freeIndexes[i];
    }
    list->freeIndexes[list->size - 1] = cDefaultIndexValue;

    return CreateSuccess();
}

Error ListAddAfterElement(List *const list, const ListElemData_t element, const unsigned count)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    int nextElementIndex = ListGetElementByCount(list, count)->nextIndex;
    int nextOfNextElementIndex = list->data[nextElementIndex].nextIndex;

    if (list->freeIndexes[0] <= 0)
    {
        return CreateError(ecNoFreeIndexesLeft, "");
    }
    int indexToPlace = list->freeIndexes[0];

    // clang-format off
    list->data[indexToPlace] =
    {
    	.data = element,
     	.prevIndex = nextElementIndex,
      	.nextIndex = nextOfNextElementIndex,
    };
    // clang-format on

    list->data[nextElementIndex].nextIndex = indexToPlace;
    list->data[nextOfNextElementIndex].prevIndex = indexToPlace;

    for (int i = 1; i < list->size; i++)
    {
        list->freeIndexes[i - 1] = list->freeIndexes[i];
    }
    list->freeIndexes[list->size - 1] = cDefaultIndexValue;

    return CreateSuccess();
}

Error ListAddBeforeTail(List *const list, const ListElemData_t element)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    return ListAddBeforeElement(list, element, 0);
}

Error ListAddAfterHead(List *const list, const ListElemData_t element)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    return ListAddAfterElement(list, element, ListGetElementCountByIndex(list, list->data[0].prevIndex));
}

Error ListRemoveElement(List *const list, const unsigned count)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    // if (list->data[index].data == cPoisonValue)
    // {
    //     char ctx[16] = {};
    //     snprintf(ctx, 15, "%d", index);
    //     return CreateError(ecNoSuchElementInList, ctx);
    // }

    // index = list->data[list->data[index].prevIndex].nextIndex;

    ListElement *elemToRemove = ListGetElementByCount(list, count);
    int indexOfElemToRemove = ListGetIndexOfElement(list, elemToRemove);

    int prevElementIndex = elemToRemove->prevIndex;
    int nextElementIndex = elemToRemove->nextIndex;

    list->data[prevElementIndex].nextIndex = nextElementIndex;
    list->data[nextElementIndex].prevIndex = prevElementIndex;

    for (int i = list->size - 1; i >= 1; i--)
    {
        list->freeIndexes[i] = list->freeIndexes[i - 1];
    }
    list->freeIndexes[0] = indexOfElemToRemove;

    list->data[indexOfElemToRemove].nextIndex = 0;
    list->data[indexOfElemToRemove].prevIndex = 0;
    list->data[indexOfElemToRemove].data = cPoisonValue;

    return CreateSuccess();
}

void ListDump(const List *const lst, const char *name, const char *fileCalledAt, const char *funcCalledAt,
              int lineCalledAt)
{
    ASSERT(lst);
    ASSERT(name);
    ASSERT(fileCalledAt);
    ASSERT(funcCalledAt);

    fprintf(stderr, "---------------------------\n");

    fprintf(stderr,
            __YELLOW "DEBUG: Dump of list \"%s\" called in function %s() at %s:%d.\n" __RESET,
            name,
            funcCalledAt,
            fileCalledAt,
            lineCalledAt);

    fprintf(stderr, __YELLOW "DEBUG: size = %u.\n" __RESET, lst->size);

    ASSERT(lst->data);
    for (int i = 0; i < lst->size; i++)
    {
        fprintf(stderr,
                __YELLOW "DEBUG: %s->data[%d]: elementValue = %d; prevIndex = %d; nextIndex = %d\n" __RESET,
                name,
                i,
                lst->data[i].data,
                lst->data[i].prevIndex,
                lst->data[i].nextIndex);
    }

    ASSERT(lst->freeIndexes);
    fprintf(stderr, __YELLOW "DEBUG: freeIndexes:\n");
    DumpIntArr(lst->freeIndexes, lst->size);
    fprintf(stderr, "DEBUG: End of \"%s\" dump.\n" __RESET, name);
    fprintf(stderr, "---------------------------\n");

    return;
}

ListElement *ListGetElementByCount(List *const list, const unsigned count)
{
    ASSERT(list);

    ListElement *elemPtr = &list->data[0];

    for (unsigned i = 0; i < count; i++)
    {
        elemPtr = &list->data[elemPtr->nextIndex];
    }

    return elemPtr;
}

int ListGetIndexOfElement(const List *const list, const ListElement *const element)
{
    ASSERT(list);
    ASSERT(element);
    ASSERT(element - list->data < list->size);

    return element - list->data;
}

unsigned ListGetElementCountByIndex(const List *const list, const int index)
{
    ASSERT(list);

    int tailIndex = list->data[0].nextIndex;
    unsigned c = 0;

    int i = index;
    while (i != tailIndex)
    {
        i = list->data[i].prevIndex;
        c++;
    }

    return c;
}
