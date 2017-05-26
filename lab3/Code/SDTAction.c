#include "syntax_tree.h"
#include "SDTAction.h"
#include <stdarg.h>
#include <malloc.h>
#include <inttypes.h>

int nextVarIndex = 1;
int nextLabelIndex = 1;
int nextFuncIndex = 1;

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

ID(4)
{
    if (childNum == 2)
    {
        D_child_1_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = child_1_info->sType;
        /* type_info->iDimension = 0; */
        child->other_info = type_info;
    }
}

ID(7)
{
    if (childNum == 1)
    {
        D_parent_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = parent_info->iType;
        type_info->iDimension = 0;

        child->other_info = type_info;
    }
}

ID(8)
{
    if (childNum == 1)
    {
        D_parent_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = parent_info->iType;
        type_info->iDimension = 0;

        child->other_info = type_info;
    }
    else if (childNum == 3)
    {
        D_parent_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = parent_info->iType;

        child->other_info = type_info;
    }
}

ID(17)
{
    if (childNum == 1)
    {
        TypeInfo *parent_info = (TypeInfo *)parent->other_info;
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iType = parent_info->iType;
        type_info->iDimension = parent_info->iDimension + 1;
        child->other_info = type_info;

        D_child_1_info;
        DNode *tmp = parent_info->dNode;
        child_1_info->dNode = malloc(sizeof(DNode));
        D_child_3;
        child_1_info->dNode->value = atoi(child_3->str + 4);
        child_1_info->dNode->next = tmp;

        D_child_1;
        DNode *tp = ((TypeInfo *)child_1->other_info)->dNode;
        /* while (tp != NULL) */
        /* { */
        /*     printf("%d\n", tp->value); */
        /*     tp = tp->next; */
        /* } */
        /* printf("\n"); */
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

ID(39)
{
    if (childNum == 1 || childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

IDS(40, 41, 42, 43, 44, 45, 46)
{
    if (childNum == 1 || childNum == 3)
    {
        TypeInfo *type_info = (TypeInfo *)malloc(sizeof(TypeInfo));
        type_info->iDimension = 0;
        child->other_info = type_info;
    }
}

ID(47)
{
    if (childNum == 2)
    {
        D_type_info;
        D_parent_info;
        type_info->iDimension = parent_info->iDimension;
        child->other_info = type_info;
    }
}

IDS(48, 49)
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
    parent_info->sValid = 1;
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

    D_parent_info;
    parent_info->sType = parent_info->iType;
    parent_info->sDimension = parent_info->iDimension;
    D_type_info;
    type_info->sType = parent_info->sType;
    type_info->sDimension = parent_info->sDimension;
    /* D_child_1_info; */
    /* child_1_info = type_info; */
    parent->first_child->other_info = type_info;

    D_child_1_info;
    child_1_info->dNode = parent_info->dNode;
    DNode *tmp = ((TypeInfo *)child_1->other_info)->dNode;
    int size = 1;
    while (tmp != NULL)
    {
        size *= tmp->value;
        tmp = tmp->next;
    }

    /* IR tag */
    child_1->IRIndex = nextVarIndex;
    nextVarIndex++;
    if (size != 1 && !isDefineFunction())
    {
        int tmpIndex = nextVarIndex;
        nextVarIndex++;
        gen_IR(Dec, new_value(V, tmpIndex), new_value(Const, size * 4));
        gen_IR(Assign, new_value(V, child_1->IRIndex), new_value(Address, tmpIndex));
    }

    if (!stackIsEmpty())
    {
        int add_result = stackAddRegion(parent->first_child->str + 4, parent->first_child);
        if (add_result == -1)
        {
            printf("Error type 15 at Line: %d: Redefine field \"%s\" in struct.\n", 
                    child_1->loc_line,
                    child_1->str + 4);
        }
        else if(add_result == 0)
        {
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", 
                child_1->loc_line, 
                child_1->str + 4);
        }
    }
    else if(isDefineFunction())
    {
        if(addTempFuncParam(parent->first_child->str + 4, type_info->sType,
            type_info->sDimension, parent->first_child) == 0)
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", 
                    child_1->loc_line, 
                    child_1->str + 4);
    }
    else
    {
        if (getSymbol(child_1->str + 4) != NULL)
        {
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", 
                    child_1->loc_line, 
                    child_1->str + 4);
            return;
        }
        addSymbol(parent->first_child->str + 4, parent->first_child);
    }
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
    if (!stackIsEmpty())
    {
        printf("Error type 15 at line %d: initialize in struct.\n", parent->first_child->loc_line);
    }
}

Value *createNewValue(int isArray, int index) {
    if (isArray) {
        return new_value(Content, index);
    }
    else {
        return new_value(V, index);
    }
}

SD(39)
{
    D_parent_info;
    /* 直接用nextInfo表征Exp是否是左值表达式 */
    /* 因为也只用0／1，所以不另外用一个结构体 */
    parent_info->nextInfo = (void*)0;
    /* (intptr_t)parent_info->nextInfo; */

    D_child_1_info;
    D_child_3_info;

    /* IR tag */
    D_child_1;
    D_child_3;
    /* gen("%c%d", 'v', child_1->IRIndex); */
    /* gen("%s", " := "); */
    /* gen("%c%d\n", 'v', child_3->IRIndex); */
    /* if (child_1_info->isArray == 1 && child_3_info->isArray == 1) */
    /* { */
    /*     gen_IR(Assign, new_value(Address, child_1->IRIndex), new_value(Address, child_3->IRIndex)); */
    /* } */
    /* else if (child_1_info->isArray == 1 && child_3_info->isArray == 0) */
    /* { */
    /*     gen_IR(Assign, new_value(Address, child_1->IRIndex), new_value(V, child_3->IRIndex)); */
    /* } */
    /* else if (child_1_info->isArray == 0 && child_3_info->isArray == 1) */
    /* { */
    /*     gen_IR(Assign, new_value(V, child_1->IRIndex), new_value(Address, child_3->IRIndex)); */
    /* } */
    /* else */
    /* { */
    /*     gen_IR(Assign, new_value(V, child_1->IRIndex), new_value(V, child_1->IRIndex)); */
    /* } */
    gen_IR(Assign, createNewValue(child_1_info->isArray, child_1->IRIndex), createNewValue(child_3_info->isArray, child_3->IRIndex));
    parent->IRIndex = nextVarIndex++;

    /* 检查赋值号左边不是左值 */
    if ((intptr_t)child_1_info->nextInfo == 0)
    {
        D_child_1;
        printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",
                child_1->loc_line);
    }

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

SDS(40)
{
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_1;
    D_child_3;
    /* gen("v%d := v%d && v%d\n", parent->IRIndex, child_1->IRIndex, child_3->IRIndex); */
    /* gen_IR(Calculate, new_value(V, parent->IRIndex), new_value(V, child_1->IRIndex), new_value(V, child_3->IRIndex), "&&"); */
    int tmpLabelIndex = nextLabelIndex;
    nextLabelIndex += 3;
    D_child_1_info;
    D_child_3_info;
    gen_IR(GotoRel, new_value(L, tmpLabelIndex), createNewValue(child_1_info->isArray, child_1->IRIndex), new_value(Const, 0), "==");
    gen_IR(GotoRel, new_value(L, tmpLabelIndex), createNewValue(child_3_info->isArray, child_3->IRIndex), new_value(Const, 0), "==");
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 1));
    gen_IR(Goto, new_value(L, tmpLabelIndex + 1));
    gen_IR(Label, new_value(L, tmpLabelIndex));
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 0));
    gen_IR(Label, new_value(L, tmpLabelIndex + 1));

    D_parent_info;
    parent_info->nextInfo = (void*)0;
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;

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

SD(41)
{
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_1;
    D_child_3;
    /* gen("v%d := v%d || v%d\n", parent->IRIndex, child_1->IRIndex, child_3->IRIndex); */
    /* gen_IR(Calculate, new_value(V, parent->IRIndex), new_value(V, child_1->IRIndex), new_value(V, child_3->IRIndex), "||"); */
    int tmpLabelIndex = nextLabelIndex;
    nextLabelIndex += 3;
    D_child_1_info;
    D_child_3_info;
    gen_IR(GotoRel, new_value(L, tmpLabelIndex), createNewValue(child_1_info->isArray, child_1->IRIndex), new_value(Const, 0), "!=");
    gen_IR(GotoRel, new_value(L, tmpLabelIndex), createNewValue(child_3_info->isArray, child_3->IRIndex), new_value(Const, 0), "!=");
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 0));
    gen_IR(Goto, new_value(L, tmpLabelIndex + 1));
    gen_IR(Label, new_value(L, tmpLabelIndex));
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 1));
    gen_IR(Label, new_value(L, tmpLabelIndex + 1));

    D_parent_info;
    parent_info->nextInfo = (void*)0;
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;

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
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_1;
    D_child_2;
    D_child_3;
    /* gen("v%d := v%d %s v%d\n", parent->IRIndex, child_1->IRIndex, child_2->str, child_3->IRIndex); */
    /* gen_IR(Calculate, new_value(V, parent->IRIndex), new_value(V, child_1->IRIndex), new_value(V, child_3->IRIndex), child_2->str); */
    int tmpIndex = nextLabelIndex;
    nextLabelIndex += 3;
    D_child_1_info;
    D_child_3_info;
    gen_IR(GotoRel, new_value(L, tmpIndex), createNewValue(child_1_info->isArray, child_1->IRIndex), 
            new_value(child_3_info->isArray, child_3->IRIndex), child_2->str);
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 0));
    gen_IR(Goto, new_value(L, tmpIndex + 1));
    gen_IR(Label, new_value(L, tmpIndex));
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 1));
    gen_IR(Label, new_value(L, tmpIndex + 1));

    D_parent_info;
    parent_info->nextInfo = (void*)0;

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
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_1;
    D_child_2;
    D_child_3;
    D_child_1_info;
    D_child_3_info;
    /* gen("v%d := v%d %s v%d\n", parent->IRIndex, child_1->IRIndex, child_2->str, child_3->IRIndex); */
    gen_IR(Calculate, new_value(V, parent->IRIndex), createNewValue(child_1_info->isArray, child_1->IRIndex),
            createNewValue(child_3_info->isArray, child_3->IRIndex), child_2->str);

    D_parent_info;
    parent_info->nextInfo = (void*)0;

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
    parent_info->nextInfo = (void*)1;

    D_child_2_info;
    parent_info->sValid = child_2_info->sValid;
    if (parent_info->sValid)
    {
        parent_info->sType = child_2_info->sType;
        parent_info->sDimension = child_2_info->sDimension;
    }


    parent_info->dNode = child_2_info->dNode;
    parent_info->isArray = child_2_info->isArray;
}

SD(48)
{
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_2;
    /* gen("v%d := -v%d\n", parent->IRIndex, child_2->IRIndex); */
    D_child_2_info;
    gen_IR(Calculate, new_value(V, parent->IRIndex), new_value(Const, 0), createNewValue(child_2_info->isArray, child_2->IRIndex), "-");

    TypeInfo* exp = (TypeInfo*)(parent->other_info);
    TypeInfo* exp_ = (TypeInfo*)(parent->first_child->next_brother->other_info);
    exp->sValid = 1;
    exp->nextInfo = (void*)0;
    exp->sType = exp_->sType;
    exp->sDimension = exp_->sDimension;
}

SD(49)
{
    /* IR tag */
    parent->IRIndex = nextVarIndex++;
    D_child_2;
    /* gen("v%d := !v%d\n", parent->IRIndex, child_2->IRIndex); */
    D_child_2_info;
    gen_IR(GotoRel, new_value(Label, nextLabelIndex + 1), createNewValue(child_2_info->isArray, child_2->IRIndex), new_value(Const, 0), "!=");
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 1));
    gen_IR(Goto, new_value(Label, nextLabelIndex + 2));
    gen_IR(Label, new_value(Label, nextLabelIndex + 1));
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, 0));
    gen_IR(Label, new_value(Label, nextLabelIndex + 2));
    nextLabelIndex += 3;
    /* gen_IR(Calculate, new_value(V, parent->IRIndex), new_value(V, child_2->IRIndex), "!"); */

    D_parent_info;
    parent_info->nextInfo = (void*)0;
    parent_info->sValid = 1;
    parent_info->sType = "int";
    parent_info->sDimension = 0;

    if (strcmp(child_2_info->sType, "int") || !(child_2_info->sDimension == 0))
    {
        D_child_2;
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", child_2->loc_line);
    }
}

SD(52)
{
    /* IR tag */
    int tmpVarIndex = nextVarIndex++;
    D_child_3;
    D_child_1;
    /* gen("v%d := v%d * %d\n", tmpVarIndex, child_3->IRIndex, 4); */
    D_parent_info;
    D_child_1_info;
    parent_info->dNode = child_1_info->dNode->next;
    DNode *tmp = parent_info->dNode;
    int offset = 1;
    while (tmp != NULL)
    {
        offset *= tmp->value;
        tmp = tmp->next;
    }
    D_child_3_info;
    gen_IR(Calculate, new_value(V, tmpVarIndex), createNewValue(child_3_info->isArray, child_3->IRIndex), new_value(Const, 4 * offset), "*");
    int tmpVarIndex2 = nextVarIndex++;
    /* gen("v%d := v%d + v%d\n", tmpVarIndex2, child_1->IRIndex, tmpVarIndex); */
    gen_IR(Calculate, new_value(V, tmpVarIndex2), new_value(V, child_1->IRIndex), new_value(V, tmpVarIndex), "+");
    parent->IRIndex = nextVarIndex++;
    /* gen("v%d := *v%d\n", parent->IRIndex, tmpVarIndex2); */
    gen_IR(Assign, new_value(V, parent->IRIndex), new_value(V, tmpVarIndex2));

    parent_info->isArray = parent_info->dNode == NULL;

    parent_info->nextInfo = (void*)1;

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
    parent_info->nextInfo = (void*)1;

    D_child_1;

    Symbol *child = getSymbolFull(child_1->str + 4);
    if (child == NULL)
    {
        printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",
               child_1->loc_line,
               child_1->str + 4);
        parent_info->sValid = 0;
        return;
    }

    /* IR tag */
    AST_node *child_ = getSymbol(child_1->str + 4);
    parent->IRIndex = child_->IRIndex;

    parent_info->dNode = ((TypeInfo *)(child_->other_info))->dNode;
    DNode *tmp = parent_info->dNode;
    /* while (tmp != NULL) */
    /* { */
    /*     printf("%d\n", tmp->value); */
    /*     tmp = tmp->next; */
    /* } */
    /* printf("\n"); */

    parent_info->sType = child->type;
    parent_info->sDimension = child->dimension - parent_info->iDimension;
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
    parent->IRIndex = nextVarIndex++;
    D_child_1;

    D_parent_info;
    parent_info->nextInfo = (void*)0;
    parent_info->sValid = 1;
    if (parent->first_child->str[0] == 'I')
    {
        parent_info->sType = "int";
        /* gen("v%d := %s\n", parent->IRIndex, child_1->str + 4); */
        gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, atoi(child_1->str + 4)));
    }
    else
    {
        parent_info->sType = "float";
        /* gen("v%d := %s\n", parent->IRIndex, child_1->str + 6); */
        gen_IR(Assign, new_value(V, parent->IRIndex), new_value(Const, atof(child_1->str + 6)));
    }
    parent_info->sDimension = 0;
}

void initTable()
{
    IS(4, 7, 8, 17, 26, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 52);
    SS(9, 16, 17, 26, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 49, 48, 52, 54, 55, 56);
}
