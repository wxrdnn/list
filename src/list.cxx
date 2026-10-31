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

Error ListAddElement(List *const list, const ListElemData_t element, const int index)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    int prevElementIndex = list->data[index].prevIndex;
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

Error ListRemoveElement(List *const list, int index)
{
    ASSERT(list);
    ASSERT(list->data);
    ASSERT(list->freeIndexes);

    if (list->data[index].data == cPoisonValue)
    {
        char ctx[16] = {};
        snprintf(ctx, 15, "%d", index);
        return CreateError(ecNoSuchElementInList, ctx);
    }

    // index = list->data[list->data[index].prevIndex].nextIndex;

    int prevElementIndex = list->data[index].prevIndex;
    int nextElementIndex = list->data[index].nextIndex;

    list->data[prevElementIndex].nextIndex = nextElementIndex;
    list->data[nextElementIndex].prevIndex = prevElementIndex;

    for (int i = list->size - 1; i >= 1; i--)
    {
        list->freeIndexes[i] = list->freeIndexes[i - 1];
    }
    list->freeIndexes[0] = index;

    list->data[index].nextIndex = 0;
    list->data[index].prevIndex = 0;
    list->data[index].data = cPoisonValue;

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
