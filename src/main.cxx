#include "h/errorHandle.h"
#include "h/list.h"

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

    SHUTDOWN_IF_ERROR_FAIL(ListInit(&lst, 5));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 20, 2));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListAddElement(&lst, 10, 2));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN_IF_ERROR_FAIL(ListRemoveElement(&lst, 3));
    ListDump(&lst, "lst", __FILE__, __FUNCTION__, __LINE__);

    SHUTDOWN(ecSuccess);
}
