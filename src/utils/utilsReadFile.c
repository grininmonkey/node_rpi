#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *read_file(const char *filename)
{

    FILE *file = fopen(filename, "rb");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, size, file);
    if (bytesRead != size)
    {
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[size] = '\0';

    fclose(file);
    return buffer;
}