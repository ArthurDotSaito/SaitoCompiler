#ifndef SAITOCOMPILER_H
#define SAITOCOMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define S_EQ(str, str2) \
    (str && str2 && (strcmp(str, str2) == 0))

struct pos
{
    int line;
    int col;
    const char *filename;
};

enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR
};

#define NUMERIC_CASE \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'

#define OPERATOR_CASE_EXCLUDING_DIVISON \
    case '+':                           \
    case '-':                           \
    case '*':                           \
    case '>':                           \
    case '<':                           \
    case '^':                           \
    case '%':                           \
    case '!':                           \
    case '=':                           \
    case '~':                           \
    case '|':                           \
    case '&':                           \
    case '(':                           \
    case '[':                           \
    case ',':                           \
    case '.':                           \
    case '?'

#define SYMBOL_CASE \
    case '{':       \
    case '}':       \
    case ':':       \
    case ';':       \
    case '#':       \
    case '\\':      \
    case ')':       \
    case ']'

enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE,
};

struct lex_process;

typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process *process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process *process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process *process, char c);

struct lex_process_functions
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
    struct pos pos;
    struct vector *token_vec;
    struct compiler_process *compiler;

    int current_expression_count;
    struct buffer *parentheses_buffer;
    struct lex_process_functions *function;

    // This will be private data that the the lexer does not understand, but the person using the lexer does.
    void *private;
};

enum
{
    NUMBER_TYPE_NORMAL,
    NUMBER_TYPE_FLOAT,
    NUMBER_TYPE_DOUBLE,
    NUMBER_TYPE_LONG,
};

struct token
{
    int type;
    int flags;
    struct pos pos;

    union
    {
        char cval;
        const char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void *any;
    };

    struct token_number
    {
        int type;
    } num;

    // It's true if there is whitespace between the token and the next token.
    // i.e * a for operator token * would mean whitespace would be set for token "a"
    bool whitespace;

    // (Hello World) Every token inside the brackets would have between_brackets pointing to the start of this expression
    // Extremely useful for debbuging
    const char *between_brackets;
};

enum
{
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS
};

struct compiler_process
{
    // flags in regards to how this file should be compiled (.exe, .obj, etc)
    int flags;

    struct pos pos;
    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;
    } cfile;

    // vector of tokens of lexical analysis
    struct vector *token_vec;

    struct vector *node_vec;
    struct vector *node_tree_vec;

    FILE *ofile;
};
enum
{
    NODE_TYPE_EXPRESSION,
    NODE_TYPE_EXPRESSION_PARENTHESES,
    NODE_TYPE_NUMBER,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_STRING,
    NODE_TYPE_VARIABLE,
    NODE_TYPE_VARIABLE_LIST,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_BODY,
    NODE_TYPE_STATEMENT_RETURN,
    NODE_TYPE_IF,
    NODE_TYPE_ELSE,
    NODE_TYPE_WHILE,
    NODE_TYPE_DO_WHILE,
    NODE_TYPE_FOR,
    NODE_TYPE_BREAK,
    NODE_TYPE_CONTINUE,
    NODE_TYPE_SWITCH,
    NODE_TYPE_CASE,
    NODE_TYPE_GOTO,
    NODE_TYPE_DEFAULT,

    NODE_TYPE_UNARY,
    NODE_TYPE_TENARY,
    NODE_TYPE_LABEL,
    NODE_TYPE_STRUCT,
    NODE_TYPE_UNION,
    NODE_TYPE_BRACKET,
    NODE_TYPE_CAST,
    NODE_TYPE_BLANK
};

enum
{
    NODE_FLAG_INSIDE_EXPRESSION = 0b00000001
};

struct node
{
    int type;
    int flags;

    struct pos pos;
    struct node_binded
    {
        struct node *owner;
        struct node *function;
    } binded;

    union
    {
        struct exp
        {
            struct node *left;
            struct node *right;
            const char *op;
        } exp;
    };

    union
    {
        char cval;
        const char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
    };
};

enum
{
    PARSE_ALL_OK,
    PARSE_GENERAL_ERROR
};

enum {
    DATATYPE_FLAG_IS_SIGNED = 0b00000001,
    DATATYPE_FLAG_IS_STATIC = 0b00000010,
    DATATYPE_FLAG_IS_CONST = 0b00000100,
    DATATYPE_FLAG_IS_POINTER = 0b00001000,
    DATATYPE_FLAG_IS_ARRAY = 0b00010000,
    DATATYPE_FLAG_IS_EXTERN = 0b00100000,
    DATATYPE_FLAG_IS_RESTRICTED = 0b01000000,
    DATATYPE_FLAG_IGNORE_TYPE_CHECKING = 0b10000000,
    DATATYPE_FLAG_IS_SECONDARY = 0b100000000,
    DATATYPE_FLAG_STRUCT_UNION_NO_NAME = 0b1000000000,
    DATATYPE_FLAG_IS_LITERAL = 0b10000000000,
};

enum {
    DATA_TYPE_VOID,
    DATA_TYPE_CHAR,
    DATA_TYPE_SHORT,
    DATA_TYPE_INTEGER,
    DATA_TYPE_LONG,
    DATA_TYPE_FLOAT,
    DATA_TYPE_DOUBLE,
    DATA_TYPE_STRUCT,
    DATA_TYPE_UNION,
    DATA_TYPE_UNKNOWN,
};

struct datatype{
    int flags;
    int type;
    struct datatype* secondary;

    const char* type_str;
    size_t size;
    int pointer_depth;

    union {
        struct node* struct_node;
        struct node* union_node;
    };

};

enum{
    DATA_TYPE_EXPECT_PRIMITIVE,
    DATA_TYPE_EXPECT_UNION,
    DATA_TYPE_EXPECT_STRUCT
};

int compile_file(const char *filename, const char *out_filename, int flags);
struct compiler_process *compile_process_create(const char *filename, const char *filename_out, int flags);

char compile_process_next_char(struct lex_process *lex_process);
char compile_process_peek_char(struct lex_process *lex_process);
void compile_process_push_char(struct lex_process *lex_process, char c);

struct lex_process *lex_process_create(struct compiler_process *compiler, struct lex_process_functions *functions, void *private);
void lex_process_free(struct lex_process *process);
void *lex_process_private(struct lex_process *process);
struct vector lex_process_tokens(struct lex_process *process);

int parse(struct compiler_process *process);

void compiler_error(struct compiler_process *compiler, const char *msg, ...);
void compiler_warning(struct compiler_process *compiler, const char *msg, ...);

int lex(struct lex_process *process);

/**
 * @brief Builds token for the input string
 *
 * @param compiler
 * @param str
 * @return struct lex_process*
 */
struct lex_process *tokens_build_for_string(struct compiler_process *compiler, const char *str);

bool token_is_keyword(struct token *token, const char *value);
bool token_is_nl_or_comment_or_newline_separator(struct token *token);
bool token_is_symbol(struct token *token, char c);

bool keyword_is_datatype(const char* string);
bool token_is_primitive_keyword(struct token* token);

struct node *node_create(struct node *_node);
void make_exp_node(struct node *left_node, struct node *right_node, const char *op);
struct node *node_pop();
struct node *node_peek();
struct node *node_peek_or_null();
void node_push(struct node *node);
void node_set_vector(struct vector *vec, struct vector *root_vec);
bool node_is_expressionable(struct node *node);
struct node *node_peek_expressionable_or_null();

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_OPERATORS_IN_GROUPS 12

enum
{
    ASSOCIATIVITY_LEFT_TO_RIGHT,
    ASSOCIATIVITY_RIGHT_TO_LEFT
};

struct expressionable_op_precedence_group
{
    char *operators[MAX_OPERATORS_IN_GROUPS];
    int associativity;
};

#endif