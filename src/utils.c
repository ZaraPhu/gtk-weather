#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int load_dotenv(char *file_path) {
    char line_buf[256];
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Could not find the .env file\n");
        return EXIT_FAILURE;
    }
    char line_number = 0;
    while(fgets(line_buf, 256, file_ptr)) {
        line_number++;
        char *val_start;
        for (int i = 0; i < 256; i++) {
            if (line_buf[i] == '=') {
                line_buf[i] = '\0';
                val_start = line_buf + sizeof(char) * (i + 2);
                break;
            }
        }
        fprintf(stdout, "%s : %s\n", line_buf, val_start);
        if (setenv(line_buf, val_start, 1) == -1) {
            fprintf(stderr, "There was an error parsing the .env file at line %d.\n", line_number);
            fclose(file_ptr);
            return EXIT_FAILURE;
        }
    }
    fclose(file_ptr);
    return EXIT_SUCCESS;
}
