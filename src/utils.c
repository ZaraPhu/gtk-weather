/*
The purpose of this file is to store the functions used in the main executable
program code for the GTK Weather app.
Author: Zara Phukan.
Creation Date: May 14, 2025.
*/

/*** Dependencies ***/
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

/*** Macro Definitions ***/
#define LINE_BUF_SIZE 256
#define RESPONSE_BUF_SIZE 1048576
#define URL_BUF_SIZE 1024
#define STR_LEN_MAX 512

/*** Global Variables/Constants ***/
static char *ACCUWEATHER_FORECAST_API = "http://dataservice.accuweather.com/forecasts/v1";
static char curl_err_buf[CURL_ERROR_SIZE];
static char curl_response[RESPONSE_BUF_SIZE];

/*** Static Functions ***/
static void parse_json(char *json_buffer) {

}

static void my_concat(char *base, const char *add) {
    /**
     * Concatenates a string to the end of another string.
     *
     * This function appends the contents of 'add' to the end of 'base'.
     * It first finds the end of the 'base' string by locating its null terminator,
     * then copies characters from 'add' to that position in 'base'.
     * The function ensures that the resulting string does not exceed STR_LEN_MAX
     * characters, including the null terminator.
     *
     * @param base Pointer to the destination string buffer
     * @param add Pointer to the null-terminated source string to be appended
     *
     * @note The 'base' buffer must be large enough to contain both strings
     *       and must be null-terminated before calling this function.
     */
    int i = 0;
    while (i < STR_LEN_MAX) {
        if (base[i] == '\0') { break; }
        i++;
    }
    int k = 0;
    while (k + i < STR_LEN_MAX) {
        base[k + i] = add[k];
        if (add[k] == '\0') { break; }
        k++;
    }
}

static char *create_request_url(
    char *location_key,
    char *api_key,
    enum ForecastFrequency frequency
) {
    char *full_url = (char *) malloc(STR_LEN_MAX);
    my_concat(full_url, ACCUWEATHER_FORECAST_API);
    char frequency_str[17];
    switch (frequency) {
        case ONE_HOUR: { my_concat(full_url, "/hourly/1hour/"); break; }
        case TWELVE_HOURS: { my_concat(full_url, "/hourly/12hour/"); break; }
        case TWENTY_FOUR_HOURS: { my_concat(full_url, "/hourly/24hour/"); break; }
        case SEVENTY_TWO_HOURS: { my_concat(full_url, "/hourly/72hour/"); break; }
        case ONE_TWENTY_HOURS: { my_concat(full_url, "/hourly/120hour/"); break; }
        case ONE_DAY: { my_concat(full_url, "/daily/1day/"); break; }
        case FIVE_DAYS: { my_concat(full_url, "/daily/5day/"); break; }
        case TEN_DAYS: { my_concat(full_url, "/daily/10day/"); break; }
        case FIFTEEN_DAYS: { my_concat(full_url, "/daily/15day/"); break; }
        default: return "INVALID";
    }
    my_concat(full_url, location_key);
    my_concat(full_url, "?apikey=");
    my_concat(full_url, api_key);
    my_concat(full_url, "%20&language=en-us&details=false&metric=false");
    return full_url;
}


/*** Exported Functions ***/
int load_dotenv(char *file_path) {
    /**
     * Loads environment variables from a file.
     *
     * This function reads a file containing environment variable definitions in the format
     * KEY=VALUE (one per line) and sets them in the current process environment.
     * Each line is parsed to separate the variable name from its value at the '=' character.
     *
     * @param file_path Path to the environment file to be loaded
     *
     * @return EXIT_SUCCESS if the environment variables were loaded successfully,
     *         EXIT_FAILURE if an error occurred (file not found, parsing error, or setenv failure)
     *
     * @note The maximum line length that can be processed is defined by LINE_BUF_SIZE.
     *       Lines exceeding this length or missing an '=' character will cause an error.
     */
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
                i = (i + 1) % LINE_BUF_SIZE;
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
    char *recv_buffer,
    size_t size,
    size_t nmemb,
    void *user_data
) {
    my_concat(curl_response, recv_buffer);
    fprintf(stdout, "Received:\n%s\n", recv_buffer);
    return size * nmemb;
}

int request_weather_data(
    char *location_key,
    char *api_key
) {
    CURL *curl_handle = curl_easy_init();
    char *full_url = create_request_url(location_key, api_key, FIVE_DAYS);
    curl_easy_setopt(curl_handle, CURLOPT_URL, full_url);
    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, curl_err_buf);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    CURLcode curl_status = curl_easy_perform(curl_handle);
    fprintf(stdout, "%s\n", full_url);

    if (curl_status)
        { fprintf(stderr, "libcurl: %s\n", curl_easy_strerror(curl_status)); return EXIT_FAILURE; }
    curl_easy_cleanup(curl_handle);
    free(full_url);
    return EXIT_SUCCESS;
}
