#include <stdio.h>
#include "compiler.h"

int main()
{
    int res = compile_file("./test.c", "./test", 0);
    if (res == COMPILER_FILE_COMPILED_OK)
    {
        printf("File compiled successfully\n");
    }
    else if (res == COMPILER_FAILED_WITH_ERRORS)
    {
        printf("File compiled with errors\n");
    }
    else
    {
        printf("File compilation failed\n");
    }

    return 0;
}