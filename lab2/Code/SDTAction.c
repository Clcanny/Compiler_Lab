#include "syntax_tree.h"
#include "SDTAction.h"
#include <stdarg.h>
#include <malloc.h>

void traversalTreePerformAction(AST_node *parent)
{
    int proNum = parent->proNum;
    sdtIActionTable[proNum](parent, parent, 0);

    int i = 1;
    for (AST_node *child = parent->first_child; 
         child != NULL;
         child = child->next_brother) 
    {
        sdtIActionTable[proNum](parent, child, i);
        traversalTreePerformAction(child);
        i++;
    }

    sdtSActionTable[proNum](parent);
}

void nullIAction(AST_node *parent, AST_node *child, int childNum)
{
    /* printf("%d\n", parent->proNum); */
}

void nullSAction(AST_node *parent)
{
}

SDTIAction sdtIActionTable[ProCount];
SDTSAction sdtSActionTable[ProCount];

typedef struct
{
    const char *iType;
    const char *sType;
    int iDimension;
    int sDimension;
} TypeInfo;

void pro34IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 2)
    {
        TypeInfo *decListInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        decListInfo->iType = parent->first_child->str;
        child->otherInformation = decListInfo;
    }
}

void pro35IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 1)
    {
        TypeInfo *decListInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        decListInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        child->otherInformation = decListInfo;
    }
}

void pro36IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 1)
    {
        TypeInfo *typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        typeInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        child->otherInformation = typeInfo;
    }
    else if (childNum == 3)
    {
        TypeInfo *typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        typeInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        child->otherInformation = typeInfo;
    }
}

void pro37IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 1)
    {
        TypeInfo *typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        typeInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        typeInfo->iDimension = 0;
        child->otherInformation = typeInfo;
    }
}

void pro37SAction(AST_node *parent)
{
    /* printf("Type valid."); */
}

void pro38IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 1)
    {
        TypeInfo *typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        typeInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        typeInfo->iDimension = 0;
        child->otherInformation = typeInfo;
    }
}

void pro38SAction(AST_node *parent)
{
    /* printf("Type valid.") */
}

void pro17IAction(AST_node *parent, AST_node *child, int childNum)
{
    if (childNum == 1)
    {
        TypeInfo *typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
        typeInfo->iType = ((TypeInfo*)parent->otherInformation)->iType;
        typeInfo->iDimension = ((TypeInfo*)parent->otherInformation)->iDimension + 1;
        child->otherInformation = typeInfo;
    }
}

void pro16SAction(AST_node *parent)
{
    ((TypeInfo*)parent)->sType = ((TypeInfo*)parent)->iType;
    ((TypeInfo*)parent)->sDimension = ((TypeInfo*)parent)->sDimension;
    TypeInfo* typeInfo = (TypeInfo*)malloc(sizeof(TypeInfo));
    typeInfo->sType = ((TypeInfo*)parent)->sType;
    typeInfo->sDimension = ((TypeInfo*)parent)->sDimension;
    parent->first_child->otherInformation = typeInfo;
    printf("ID's base type is %s, dimension is %d\n", typeInfo->sType, typeInfo->sDimension);
    /* addSymbol(parent->first_child); */
}

void pro17SAction(AST_node *parent)
{
    ((TypeInfo*)parent)->sType = ((TypeInfo*)parent->first_child)->iType;
    ((TypeInfo*)parent)->sDimension = ((TypeInfo*)parent->first_child)->sDimension;
}

void initTable()
{
    for (int i = 0; i < ProCount; i++)
    {
        sdtIActionTable[i] = nullIAction;
    }
    sdtIActionTable[17] = pro17IAction;
    sdtIActionTable[34] = pro34IAction;
    sdtIActionTable[35] = pro35IAction;
    sdtIActionTable[36] = pro36IAction;
    sdtIActionTable[37] = pro37IAction;
    sdtIActionTable[38] = pro38IAction;
    for (int i = 0; i < ProCount; i++)
    {
        sdtSActionTable[i] = nullSAction;
    }
    sdtSActionTable[16] = pro16SAction;
    sdtSActionTable[17] = pro17SAction;
    sdtSActionTable[37] = pro37SAction;
    sdtSActionTable[38] = pro38SAction;
}
