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
    // DONE TODO ListAddBeforeElement()
    // DONE TODO ListAddAfterElement()
    // TODO Sort list
    // TODO List verification
    // TODO HTML log file
    // DONE TODO Fix system()
    // DONE TODO Dump in order of index, not count

    List lst = {};

    FILE *fp = fopen(OUTPUT_FILE_NAME, "w");

    SHUTDOWN_IF_ERROR_FAIL(ListInit(&lst, 10));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddBeforeTail(&lst, 10));
    SHUTDOWN_IF_ERROR_FAIL(ListAddBeforeTail(&lst, 20));
    SHUTDOWN_IF_ERROR_FAIL(ListAddBeforeTail(&lst, 30));
    SHUTDOWN_IF_ERROR_FAIL(ListAddBeforeTail(&lst, 40));
    // SHUTDOWN_IF_ERROR_FAIL(ListAddBeforeHead(&lst, 50));

    SHUTDOWN_IF_ERROR_FAIL(BuildGraph(&lst, fp));

    fclose(fp);

    int status = system("dot -Tpng \"" OUTPUT_FILE_NAME "\" -o \"" OUTPUT_FILE_NAME ".png\"");

    // fprintf(stderr, "DEBUG: system() command: dot -Tpng \"" OUTPUT_FILE_NAME "\" -o \"" OUTPUT_FILE_NAME ".png\"\n");
    // fprintf(stderr, "DEBUG: system() status: %d\n", status);

    SHUTDOWN(ecSuccess);
}
