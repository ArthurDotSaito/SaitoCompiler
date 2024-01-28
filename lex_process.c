#include "compiler.h"
#include <stdlib.h>
#include "helpers/vector.h"

struct lex_process *lex_process_create(struct compile_process *compiler, struct lex_process_functions *functions, void *private)
{
    struct lex_process *process = calloc(1, sizeof(struct lex_process));
    process->function = functions;
    process->token_vec = vector_create(sizeof(struct token));
    process->compiler = compiler;
    process->private = private;
    process->pos.line = 1;
    process->pos.col = 1;

    return process;
};
