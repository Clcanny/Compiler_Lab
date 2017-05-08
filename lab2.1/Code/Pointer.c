#include "Pointer.h"
#include "SDTAction.h"
#include <malloc.h>
/* #include <assert.h> */
/* #define assert(exp) ; */

bool isChildProduction(AST_node *node, int proNum)
{
    node = node->first_child;
    if (node == NULL)
    {
	return false;
    }
    for (; node != NULL; node = node->next_brother)
    {
	if (node->proNum == proNum)
	{
	    return true;
	}
    }
    return false;
}

typedef struct PL
{
    int allocProNum;
    AllocatorRole role;
    int deallocProNum;
    void **pointer;
    struct PL *nextPointerLog;
} PointerLog;

static PointerLog *pl = NULL;

void alloc(int proNum, AllocatorRole role, void **pointer, void *value)
{
    assert(value != NULL);
    assert(*pointer == NULL);
    *pointer = value;

    PointerLog *newPl = (PointerLog *)malloc(sizeof(PointerLog));
    newPl->allocProNum = proNum;
    newPl->role = role;
    newPl->deallocProNum = UNALLOC;
    newPl->pointer = pointer;
    newPl->nextPointerLog = NULL;

    if (pl == NULL)
    {
	pl = newPl;
    }
    else
    {
	PointerLog *tmp = pl;
	while (tmp->nextPointerLog != NULL)
	{
	    tmp = tmp->nextPointerLog;
	}
	tmp->nextPointerLog = newPl;
    }
}

void overwrite(int proNum, AllocatorRole role, void **pointer, void *value)
{
    assert(*pointer != NULL);
    assert(value != NULL);

    PointerLog *tmp;
    for (tmp = pl; tmp != NULL; tmp = tmp->nextPointerLog)
    {
	if (tmp->pointer == pointer)
	{
	    tmp->deallocProNum = OVERWRITE;
	    break;
	}
    }
    assert(tmp != NULL);

    *pointer = NULL;
    alloc(proNum, role, pointer, value);
}

void dealloc(int proNum, void **pointer)
{
    *pointer = NULL;
    PointerLog *tmp;
    for (tmp = pl; tmp != NULL; tmp = tmp->nextPointerLog)
    {
	if (tmp->pointer == pointer)
	{
	    tmp->deallocProNum = proNum;

	    *pointer = NULL;
	    return;
	}
    }
#ifdef pointer_debug_print
    printf("fuck\n");
#endif
}

void printPointerLog()
{
    PointerLog *tmp;
    for (tmp = pl; tmp != NULL; tmp = tmp->nextPointerLog)
    {
	printf("%p alloc: %d dealloc: %d.\n", tmp->pointer, tmp->allocProNum, tmp->deallocProNum);
    }
}
