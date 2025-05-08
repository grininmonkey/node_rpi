#include <stdio.h>

void write_to_file(const char *filename, const char *line, const char *mode) {
    // Validate mode input
    if (mode == NULL || (mode[0] != 'w' && mode[0] != 'a') || mode[1] != '\0') {
        fprintf(stderr, "Invalid mode. Use 'w' for write or 'a' for append.\n");
        return;
    }

    if (filename == NULL || line == NULL) {
        fprintf(stderr, "Invalid filename or line: filename=%p, line=%p\n", filename, line);
        return;
    }

    FILE *file = fopen(filename, mode); // Open file in specified mode
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "%s\n", line); // Write the line to the file
    fclose(file);
}