/*
The purpose of this file is to store the functions used in the main executable
program code for the GTK Weather app.
Author: Zara Phukan.
Creation Date: May 14, 2025.
*/

/*** Dependencies ***/
#include "utils.h"
#include <curl/easy.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

/*** Macro Definitions ***/
#define LINE_BUF_SIZE 256
#define RESPONSE_BUF_SIZE 1024

/*** Global Variables/Constants ***/
static char *ACCUWEATHER_FORECAST_API = "developer.accuweather.com/accuweather-forecast-api/apis/get";
static char *curl_err_buf[CURL_ERROR_SIZE];

/*** Static Functions ***/
static char *create_request_url() {
    return strcat(ACCUWEATHER_FORECAST_API, "/forecasts/v1/daily/1day/49538?apikey=dnSMVc7YVaMK3qx9VQXH98w1ARJWAwn1&language=en-us&details=false&metric=false");
}

/*** Exported Functions ***/
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
    char *full_url = create_request_url();
    curl_easy_setopt(handle, CURLOPT_HTTPGET, full_url);
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, curl_err_buf);
    CURLcode status = curl_easy_perform(handle);
    if (status) { fprintf(stderr, "libcurl: %s\n", curl_easy_strerror(status)); return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
