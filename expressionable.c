#include "compiler.h"

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_OPERATORS_IN_GROUPS 12

enum
{
    ASSOCIATIVITY_LEFT_TO_RIGHT,
    ASSOCIATIBITY_RUGHT_TO_LEFT
};

struct expressionable_op_precedence_group
{
    char *operators[MAX_OPERATORS_IN_GROUPS];
    int associativity;
};

struct expressionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = {
    {.operators = {"++", "--", "()", "[]", "(", "[", ".", "->", NULL}, .associativity = ASSOCIATIVITY_LEFT_TO_RIGHT}};