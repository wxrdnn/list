#include "h/graphing.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/list.h"
#include <cstdio>

Error BuildGraph(const List *const list, FILE *const output)
{
    ASSERT(list);
    ASSERT(output);

    fprintf(output,
            "digraph G {\n"
            "nodesep=1;\n"
            "ranksep=1;\n"
            "splines=ortho;\n"
            "subgraph List {\n"
            "rank=same;\n");

    for (int i = 1; i < list->size; i++)
    {
        if (list->data[i].data == cPoisonValue)
        {
            continue;
        }
        fprintf(output, "node_%d -> node_%d [style=invis];\n", i - 1, i);
    }

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
                "color=\"#008000\";\n"
                "weight=50;\n");

        fprintf(output,
                "label=\"{ index = %d | count = %d | data = %d | { prev = %d | next = %d } }\";\n",
                i,
                i == 0 ? -1 : ListGetElementCountByIndex(list, i),
                list->data[i].data,
                list->data[i].prevIndex,
                list->data[i].nextIndex);

        fprintf(output, "];\n");

        fprintf(output, "node_%d -> node_%d\n", i, list->data[i].nextIndex);
        // fprintf(output, "node_%d -> node_%d\n", i, i + 1);
    }

    fprintf(output, "}\n");

    fprintf(output, "\nHEAD -> node_%d\n", list->data[0].prevIndex);
    fprintf(output, "TAIL -> node_%d\n", list->data[0].nextIndex);

    fprintf(output, "}\n");

    return CreateSuccess();
}
