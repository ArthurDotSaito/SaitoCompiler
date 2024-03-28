#include "compiler.h"
#include "helpers/vector.h"

static struct compile_process *current_process;

int parse(struct compiler_process *process)
{
    current_process = process;
    vector_set_peek_pointer(process->token_vec, 0);

    struct node *node = NULL;
    while (parse_next() == 0)
    {
        node = node_peek();
        vector_push(process->node_vec, &node);
    }
    return PARSE_ALL_OK;
}