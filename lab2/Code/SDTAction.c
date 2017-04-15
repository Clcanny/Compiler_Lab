#include "syntax_tree.h"
#include "SDTAction.h"
#include <stdarg.h>
#include <malloc.h>
//#define SDT_DEBUG_PRINT

void traversalTreePerformAction(AST_node *parent)
{
    int proNum = parent->proNum;
    
    SDTIAction f = sdtIActionTable[proNum];
    if (f != NULL)
    {
        f(parent, parent, 0);
    }
// #ifdef SDT_DEBUG_PRINT
//     print_child_node(parent, 0);
// #endif
    int i = 1;
#ifdef SDT_DEBUG_PRINT
    printf("Line %d: %s childNum(%d) (%d) I Action start.\n",
        parent->loc_line, parent->str, i, proNum);
#endif
    for (AST_node *child = parent->first_child;
            child != NULL;
            child = child->next_brother)
    {
        if (f != NULL)
        {
            f(parent, child, i);
        }
        traversalTreePerformAction(child);
        i++;
    }
#ifdef SDT_DEBUG_PRINT
    printf("Line %d: %s (%d) I Action end.\n", parent->loc_line, parent->str, proNum);
#endif
    
    SDTSAction g = sdtSActionTable[proNum];
    if (g != NULL)
    {
        g(parent);
    }
#ifdef SDT_DEBUG_PRINT
    printf("Line %d: %s (%d) S Action end.\n", parent->loc_line, parent->str, proNum);
#endif
}

SDTIAction sdtIActionTable[ProCount + 1] = { NULL };
SDTSAction sdtSActionTable[ProCount + 1] = { NULL };

// typedef struct
// {
//     const char *iType;
//     const char *sType;
//     int iDimension;
//     int sDimension;
//     int sValid;
// } TypeInfo;

ID(17)
{
    if (childNum == 1)
    {
        TypeInfo *parent_info = (TypeInfo *)parent->other_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = parent_info->iType;
        type_info->iDimension = parent_info->iDimension + 1;
        child->other_info = type_info;
    }
}

ID(26)
{
    if (childNum == 1)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

ID(34)
{
    if (childNum == 2)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = ((TypeInfo *)parent->first_child->other_info)->sType;
        child->other_info = type_info;
        /* printf("%s\n", type_info->iType); */
    }
}

ID(35)
{
    if (childNum == 1)
    {
        TypeInfo *decListInfo = (TypeInfo *)malloc(sizeof(TypeInfo));
        decListInfo->iType = ((TypeInfo *)parent->other_info)->iType;
        child->other_info = decListInfo;
    }
}

ID(36)
{
    if (childNum == 1)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = ((TypeInfo *)parent->other_info)->iType;
        child->other_info = type_info;
    }
    else if (childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = ((TypeInfo *)parent->other_info)->iType;
        child->other_info = type_info;
    }
}

ID(37)
{
    if (childNum == 1)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = ((TypeInfo *)parent->other_info)->iType;
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

ID(38)
{
    if (childNum == 1)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = ((TypeInfo *)parent->other_info)->iType;
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
    else if (childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

IDS(39, 40, 41, 42, 43, 44, 45, 46)
{
    if (childNum == 1 || childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

IDS(47, 48, 49)
{
    if (childNum == 2)
    {
        D_type_info;
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

ID(52)
{
    if (childNum == 1)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = ((TypeInfo *)parent->other_info)->iDimension + 1;
        child->other_info = type_info;
    }
    else if (childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

SD(9)
{
    TypeInfo *parent_info = (TypeInfo *)malloc(sizeof(TypeInfo));
    parent_info->sType = parent->first_child->str;
    /* D_type_info; */
    /* type_info->sType = parent->first_child->str + 6; */
    /* D_parent_info; */
    parent_info = parent_info;
    parent->other_info = parent_info;
    /* printf("%s\n", parent_info->sType); */
}

SD(16)
{
    D_child_1;
    if (getSymbol(child_1->str + 4) != NULL)
    {
        printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", 
                child_1->loc_line, 
                child_1->str + 4);
        return;
    }
    D_parent_info;
    parent_info->sType = parent_info->iType;
    parent_info->sDimension = parent_info->iDimension;
    D_type_info;
    type_info->sType = parent_info->sType;
    type_info->sDimension = parent_info->sDimension;
    /* D_child_1_info; */
    /* child_1_info = type_info; */
    parent->first_child->other_info = type_info;
    addSymbol(parent->first_child->str + 4, parent->first_child);
}

SD(17)
{
    D_parent_info;
    D_child_1_info;
    parent_info->sType = child_1_info->iType;
    parent_info->sDimension = child_1_info->sDimension;
}

SD(26)
{
    TypeInfo *child_info = (TypeInfo *)parent->first_child->other_info;
}

SD(37)
{
}

SD(38)
{
    D_child_1_info;
    D_child_3_info;
    if (!child_3_info->sValid || 
         child_1_info->sDimension != child_3_info->sDimension || 
         strcmp(child_1_info->sType, child_3_info->sType))
    {
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n", parent->first_child->loc_line);
    }
}

SD(39)
{
    D_parent_info;
    D_child_1_info;
    D_child_3_info;

    if (child_1_info->sValid && child_3_info->sValid)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_1_info->sType;
        parent_info->sDimension = child_1_info->sDimension;
        if (strcmp(child_1_info->sType, child_3_info->sType) || child_1_info->sDimension != child_3_info->sDimension)
        {
            D_child_1;
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n", 
                    child_1->loc_line);
        }
    }
    else if (child_1_info->sValid)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_1_info->sType;
        parent_info->sDimension = child_1_info->sDimension;

        D_child_3;
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n", 
                child_3->loc_line);
    }
    else if (child_3_info->sValid)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_3_info->sType;
        parent_info->sDimension = child_3_info->sDimension;

        D_child_1;
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n", 
                child_1->loc_line);
    }
    else
    {
        parent_info->sValid = 0;

        D_child_1;
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n", 
                child_1->loc_line);
    }
}

SDS(40, 41)
{
    D_parent_info;
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;

    D_child_1_info;
    D_child_3_info;
    int v = child_1_info->sValid && child_3_info->sValid;
    if (v)
    {
        v &= strcmp(child_1_info->sType, "int");
        v &= strcmp(child_3_info->sType, "int");
        v &= child_1_info->sDimension == 0;
        v &= child_3_info->sDimension == 0;
    }
    if (!v)
    {
        D_child_1;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n",
                child_1->loc_line);
    }
}

SD(42)
{
    D_parent_info;
    D_child_1_info;
    D_child_3_info;
    int v1 = child_1_info->sValid && 
    /* int v1 = */
             (!strcmp(child_1_info->sType, "int") || !strcmp(child_1_info->sType, "float")) && 
             child_1_info->sDimension == 0;
    int v3 = child_3_info->sValid && 
    /* int v3 = */
             (!strcmp(child_3_info->sType, "int") || !strcmp(child_3_info->sType, "float")) && 
             child_3_info->sDimension == 0;
    int v = v1 && v3 && !strcmp(child_1_info->sType, child_3_info->sType);
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;
    if (!v)
    {
        D_child_1;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n",
                child_1->loc_line);
    }
}

SDS(43, 44, 45, 46)
{
    D_parent_info;
    D_child_1_info;
    D_child_3_info;
    int v1 = child_1_info->sValid && 
             (!strcmp(child_1_info->sType, "int") || !strcmp(child_1_info->sType, "float")) && 
             child_1_info->sDimension == 0;
    int v3 = child_3_info->sValid && 
             (!strcmp(child_3_info->sType, "int") || !strcmp(child_3_info->sType, "float")) && 
             child_3_info->sDimension == 0;
    if (v1 && v3)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_1_info->sType;
        parent_info->sDimension = child_1_info->sDimension;

        if (strcmp(child_1_info->sType, child_3_info->sType) ||
            child_1_info->sDimension != child_3_info->sDimension)
        {
            D_child_1;
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_1->loc_line);
        }
    }
    else if(v1)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_1_info->sType;
        parent_info->sDimension = child_1_info->sDimension;

        D_child_3;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_3->loc_line);
    }
    else if(v3)
    {
        parent_info->sValid = 1;
        parent_info->sType = child_3_info->sType;
        parent_info->sDimension = child_3_info->sDimension;

        D_child_1;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_1->loc_line);
    }
    else
    {
        parent_info->sValid = 0;

        D_child_1;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_1->loc_line);
    }
}

SD(47)
{
    D_parent_info;
    D_child_2_info;
    parent_info->sValid = child_2_info->sValid;
    if (parent_info->sValid)
    {
        parent_info->sType = child_2_info->sType;
        parent_info->sDimension = child_2_info->sDimension;
    }
}

SD(48)
{
    TypeInfo* exp = (TypeInfo*)(parent->other_info);
    TypeInfo* exp_ = (TypeInfo*)(parent->first_child->next_brother->other_info);
    exp->sValid = 1;
    exp->sType = exp_->sType;
    exp->sDimension = exp_->sDimension;
}

SD(49)
{
    D_parent_info;
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;

    D_child_2_info;
    if (strcmp(child_2_info->sType, "int") || !(child_2_info->sDimension == 0))
    {
        D_child_2;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_2->loc_line);
    }
}

SD(52)
{
    D_parent_info;
    D_child_1_info;
    D_child_3_info;
    if (!(child_3_info->sDimension == 0 && !strcmp(child_3_info->sType, "int")))
    {
        D_child_3;
        printf("Error type 12 at Line %d: \"%s\" is not an integer.\n", 
                child_3->loc_line, 
                child_3->str);
    }
    parent_info->sValid = child_1_info->sValid;
    parent_info->sType = child_1_info->sType;
    parent_info->sDimension = child_1_info->sDimension;
}

SD(54)
{
    D_parent_info;

    D_child_1;
    AST_node *child = getSymbol(child_1->str + 4);
    if (child == NULL)
    {
        printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",
               child_1->loc_line,
               child_1->str + 4);
        parent_info->sValid = 0;
        return;
    }

    D_child_info;
    parent_info->sType = child_info->sType;
    parent_info->sDimension = child_info->sDimension - parent_info->iDimension;
    if (parent_info->sDimension < 0)
    {
        parent_info->sValid = 0;
        printf("Error type 10 at Line %d: \"%s\" is not an array.\n", 
                child_1->loc_line, 
                child_1->str + 4);
        return;
    }
    parent_info->sValid = 1;
}

SDS(55, 56)
{
    D_parent_info;
    parent_info->sValid = 1;
    if (parent->first_child->str[0] == 'I')
    {
        parent_info->sType = "int";
    }
    else
    {
        parent_info->sType = "float";
    }
    parent_info->sDimension = 0;
}

void initTable()
{
    IS(17, 26, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 52);
    SS(9, 16, 17, 26, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 49, 48, 52, 54, 55, 56);
}
