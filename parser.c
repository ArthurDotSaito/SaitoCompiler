#include "compiler.h"
#include "helpers/vector.h"

static struct compiler_process *current_process;

static struct token *parser_last_token;

static void parser_ignore_nl_or_comment(struct token *token)
{
    while (token && token_is_nl_or_comment_or_newline_separator(token))
    {
    }
}

static struct token *token_next()
{
    struct token *next_token = vector_peek_no_increment(current_process->token_vec);
    parser_ignore_nl_or_comment(next_token);
    current_process->pos = next_token->pos;
    parser_last_token = next_token;
    return vector_peek(current_process->token_vec);
}

int parse_next()
{
    return 0;
}

int parse(struct compiler_process *process)
{
    current_process = process;
    vector_set_peek_pointer(process->token_vec, 0);

    struct node *node = NULL;
    while (parse_next() == 0)
    {
        // node = node_peek();
        vector_push(process->node_vec, &node);
    }
    return PARSE_ALL_OK;
}