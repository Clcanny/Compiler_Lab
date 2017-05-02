#include "SDTAction.h"
#include "symbol_table.h"
#include "7.h"

/* ADT */
/* ADTDef */
/*     : ADTHeader %prec LOWER_THAN_ASSIGNOP SEMI { $$ = new_parent_node("ADTRef", GROUP_7 + 1, 1, $1); } */
/*     | ADTHeader ASSIGNOP ConstructorDecList { $$ = new_parent_node("ADTHeader", GROUP_7 + 2, 2, $1, $3); } */
/*     ; */
ID(702)
{
    if (childNum == 2)
    {
        D_child_1_info;
        assert(child_1_info != NULL);
        assert(child->other_info == NULL);
        /* child->oter_info充当继承属性 */
        child->other_info = child_1_info;
    }
}
SD(702)
{
    D_child_1;
    D_child_2;
    child_1->other_info = NULL;
    /* “回收”继承属性对应的域 */
    child_2->other_info = NULL;
}

/* ADTHeader */
/*     : DATA TypeId ADTParamList { $$ = new_parent_node("ADTHeader", GROUP_7 + 3, 2, $2, $3); } */
/*     ; */
ID(703)
{
    if (childNum == 2)
    {
        D_child_1_info;
        assert(child_1_info != NULL);
        assert(child->other_info == NULL);
        child->other_info = child_1_info;
    }
}
SD(703)
{
    D_child_1_info;
    assert(child_1_info != NULL);
    assert(parent->other_info == NULL);
    parent->other_info = child_1_info;
}

/* ADTParamList */
/*     : ADTParam ADTParamList { $$ = new_parent_node("ADTParamList", GROUP_7 + 4, 2, $1, $2); } */
/*     | /1* empty *1/ { $$ = new_parent_node("ADTParamList", GROUP_7 + 5, 0); } */
/*     ; */
ID(704)
{
    if (childNum ==  1 || childNum == 2)
    {
        child->other_info = parent->other_info;
    }
}
SD(704)
{
    D_child_1_info;
    D_child_1;
    addSymbol((char *)child_1_info, child_1);

    D_child_2;
    child_1->other_info = NULL;
    child_2->other_info = NULL;
}

/* ADTParam */
/*     : LOWERID { $$ = new_parent_node("ADTParam", GROUP_7 + 6, 1, $1); } */
/*     ; */
SD(706)
{
    D_parent_info;
    D_child_1;
    /* TypeId-ADTParam\0 */
    assert(parent_info != NULL);
    int part1 = strlen((char *)parent_info);
    int part2 = strlen(child_1->str + 4);
    char *name = malloc((part1 + 1 + part2 + 1) * sizeof(char));
    strcpy(name, (char *)parent_info);
    name[part1] = '-';
    strcpy(name + part1 + 1, child_1->str + 4);
    /* parent->other_info由继承属性变为综合属性 */
    parent->other_info = name;
}

/* ConstructorDecList */
/*     : ConstructorDec SEMI { $$ = new_parent_node("ConstructorDecList", GROUP_7 + 7, 1, $1); } */
/*     | ConstructorDec SEMI ConstructorDecList { $$ = new_parent_node("ConstructorDecList", GROUP_7 + 8, 2, $1, $3); } */ 
/*     ; */
ID(707)
{
    if (childNum == 1)
    {
        D_parent_info;
        child->other_info = parent_info;
    }
}
SD(707)
{
    D_child_1;
    child_1->other_info = NULL;
}

ID(708)
{
    if (childNum == 1 || childNum == 2)
    {
        D_parent_info;
        child->other_info = parent_info;
    }
}
SD(708)
{
    D_child_1;
    D_child_2;
    child_1->other_info = NULL;
    child_2->other_info = NULL;
}

/* ConstructorDec */
/*     : ConstructorId TypeIdList { $$ = new_parent_node("ConstructorDec", GROUP_7 + 9, 2, $1, $2); } */
/*     ; */
ID(709)
{
    if (childNum == 2)
    {
        D_parent_info;
        child->other_info = parent_info;
    }
}
SD(709)
{
    D_child_2;
    child_2->other_info = NULL;
}

/* ConstructorId */
/*     : UPPERID { $$ = new_parent_node("ConstructorId", GROUP_7 + 10, 1, $1); } */
/*     ; */
SD(710)
{
    D_child_1;
    addSymbol(child_1->str + 4, parent);
}

/* TypeIdList */
/*     : TypeId TypeIdList { $$ = new_parent_node("TypeIdList", GROUP_7 + 11, 2, $1, $2); } */
/*     | ADTParam TypeIdList { $$ = new_parent_node("TypeIdList", GROUP_7 + 12, 2, $1, $2); } */
/*     | /1* empty *1/ { $$ = new_parent_node("TypeIdList", GROUP_7 + 13, 0); } */
/*     ; */
ID(711)
{
    if (childNum == 2)
    {
        D_parent_info;
        child->other_info = parent_info;
    }
}
SD(711)
{
    D_child_2;
    child_2->other_info = NULL;
}

ID(712)
{
    if (childNum == 1 || childNum == 2)
    {
        D_parent_info;
        child->other_info = parent_info;
    }
}
SD(712)
{
    D_child_1;
    D_child_2;
    child_1->other_info = NULL;
    child_2->other_info = NULL;
}

/* TypeId */
/*     : UPPERID { $$ = new_parent_node("TypeId", GROUP_7 + 14, 1, $1); } */
/*     ; */
SD(714)
{
    D_child_1;
    addSymbol(child_1->str + 4, parent);
    /* 把名字继承到父亲 */
    parent->other_info = (void *)child_1->str + 4;
}

/* pattern matching */
/* PatternMatching */
/*     : LET LP ConstructorId PatternMatchingParamList RP ASSIGNOP VarDec { */ 
/*         $$ = new_parent_node("PatternMatching", GROUP_7 + 15, 3, $3, $4, $7); */ 
/*     } */
/*     ; */
/* PatternMatchingParamList */
/*     : LOWERID PatternMatchingParamList { */ 
/*         $$ = new_parent_node("PatternMatchingParamList", GROUP_7 + 16, 2, $1, $2); */
/*     } */
/*     | PLACEHOLDER PatternMatchingParamList { */ 
/*         $$ = new_parent_node("PatternMatchingParamList", GROUP_7 + 17, 2, $1, $2); */ 
/*     } */
/*     | /1* empty *1/ { $$ = new_parent_node("PatternMatchingParamList", GROUP_7 + 18, 0); } */
/*     ; */

void initActionTable7()
{
    IS(702, 703, 704, 707, 708, 709, 711, 712);
    SS(702, 703, 704, 706, 707, 708, 709, 710, 711, 712, 714);
}
