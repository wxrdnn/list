#include "h/graphing.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/list.h"
#include <cstdio>

Error BuildGraph(const List *const list, FILE *const output)
{
    ASSERT(list);
    ASSERT(output);

    fprintf(output, "digraph G {\n");

    for (int i = 0; i < list->size; i++)
    {
        if (list->data[i].data == cPoisonValue)
        {
            continue;
        }

        fprintf(output, "node_%d [\n", i);

        fprintf(output,
                "shape=Mrecord;\n"
                "style=\"filled\";\n"
                "fillcolor=\"#C0FFC0\";\n"
                "color=\"#008000\";\n");

        fprintf(output,
                "label=\"{ index = %d | data = %d | { prev = %d | next = %d } }\";\n",
                i,
                list->data[i].data,
                list->data[i].prevIndex,
                list->data[i].nextIndex);

        fprintf(output, "];\n");

        fprintf(output, "node_%d -> node_%d\n", i, list->data[i].nextIndex);
    }

    fprintf(output, "}\n");

    return CreateSuccess();
}
