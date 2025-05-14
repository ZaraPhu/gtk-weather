#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define LINE_BUF_SIZE 256

int load_dotenv(char *file_path) {
    char line_buf[LINE_BUF_SIZE];
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Could not find the .env file\n");
        return EXIT_FAILURE;
    }

    uint8_t line_number = 0;
    while(fgets(line_buf, LINE_BUF_SIZE, file_ptr)) {
        line_number++;
        uint8_t i = 0;
        while (i < LINE_BUF_SIZE) {
            if (line_buf[i] == '=') {
                line_buf[i] = '\0';
                i = (i + 2) % LINE_BUF_SIZE;
                break;
            }
            i++;
        }
        if (setenv(line_buf, line_buf + (i * sizeof(char)), 1) == -1) {
            fprintf(stderr, "There was an error parsing the .env file at line %d.\n", line_number);
            fclose(file_ptr);
            return EXIT_FAILURE;
        }
    }
    fclose(file_ptr);
    return EXIT_SUCCESS;
}

void load_css(char* css_file_path) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GFile *css_file = g_file_new_for_path(css_file_path);
    gtk_css_provider_load_from_file(provider, css_file);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), 1);
}
