#ifndef SAITOCOMPILER_H
#define SAITOCOMPILER_H

struct compile_process
{
    // flags in regards to how this file should be compiled (.exe, .obj, etc)
    int flags;
};
int compile_file(const char *filename, const char *out_filename, int flags);

#endif