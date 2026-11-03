#include "h/errorHandle.h"
#include "h/graphing.h"
#include "h/list.h"
#include <cstdio>
#include <cstdlib>

#define OUTPUT_FILE_NAME "../logs/" __DATE__ "_" __TIME__ ".gv"

#define SHUTDOWN(__exitCode)                                                                                           \
    {                                                                                                                  \
        ListFree(&lst);                                                                                                \
        return __exitCode;                                                                                             \
    }

#define SHUTDOWN_IF_ERROR_FAIL(__error)                                                                                \
    {                                                                                                                  \
        Error __err = __error;                                                                                         \
        if (__err.exitCode != ecSuccess)                                                                               \
        {                                                                                                              \
            HandleError(__err);                                                                                        \
            SHUTDOWN(__err.exitCode);                                                                                  \
        }                                                                                                              \
    }
int main()
{
    List lst = {};

    FILE *fp = fopen(OUTPUT_FILE_NAME, "w");

    SHUTDOWN_IF_ERROR_FAIL(ListInit(&lst, 10));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 10, 1));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 20, 2));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 30, 3));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 22, 2));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 40, 4));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 50, 5));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListRemoveElement(&lst, 2));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(BuildGraph(&lst, fp));

    int status = system("dot -Tpng \"" OUTPUT_FILE_NAME // FIXME !!!!!!
                        "\" -o \"" OUTPUT_FILE_NAME ".png\"");

    fprintf(stderr, "DEBUG: system() command: dot -Tpng \"" OUTPUT_FILE_NAME "\" -o \"" OUTPUT_FILE_NAME ".png\"\n");
    fprintf(stderr, "DEBUG: system() status: %d\n", status);

    SHUTDOWN(ecSuccess);
}
