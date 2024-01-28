#ifndef SAITOCOMPILER_H
#define SAITOCOMPILER_H

#include <stdio.h>
#include <stdbool.h>

struct token
{
    int type;
    int flags;

    union
    {
        char cval;
        const char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void *any;
    };

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

struct compile_process
{
    // flags in regards to how this file should be compiled (.exe, .obj, etc)
    int flags;

    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;
    } cfile;

    FILE *ofile;
};
int compile_file(const char *filename, const char *out_filename, int flags);
struct compile_process *compile_process_create(const char *filename, const char *filename_out, int flags);

#endif