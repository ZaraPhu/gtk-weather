#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

#define LINE_BUF_SIZE 256
#define RESPONSE_BUF_SIZE 1024

static struct curl_slist* generate_headers(

) {
    return NULL;
}

int load_dotenv(char *file_path) {
    char line_buf[LINE_BUF_SIZE];
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Could not load environment file. %s '%s'.\n", strerror(errno), file_path);
        return EXIT_FAILURE;
    }

    uint8_t line_number = 0;
    while(fgets(line_buf, LINE_BUF_SIZE, file_ptr)) {
        line_number++;
        uint8_t i = 0;
        while (true) {
            if (line_buf[i] == '=') {
                line_buf[i] = '\0';
                i = (i + 2) % LINE_BUF_SIZE;
                break;
            }
            if (i == (LINE_BUF_SIZE - 1)) {
                fprintf(stderr, "Reached end of line %d without finding environment variable declaration.", line_number);
                fclose(file_ptr);
                return EXIT_FAILURE;
            }
            i++;
        }
        if (setenv(line_buf, line_buf + (i * sizeof(char)), 1) == -1) {
            fprintf(stderr, "%s.\n", strerror(errno));
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

size_t write_data(
    char *buffer,
    size_t len,
    size_t nmemb,
    void *user_data
) {
    return 1;
}

int request_weather_data(
    CURL *handle,
    char *location_key,
    char *api_key
) {
    struct curl_slist *headers = generate_headers(

    );
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_HTTPGET, "https://www.google.com");
    curl_slist_free_all(headers);
    return EXIT_SUCCESS;
}
