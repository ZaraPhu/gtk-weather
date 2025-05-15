#include <curl/curl.h>
#include <stddef.h>

#ifndef UTILS_H
#define UTILS_H

int load_dotenv(char *file_path);
void load_css(char *css_file_path);
size_t write_data(char *buffer, size_t len, size_t nmemb, void *user_data);
int request_weather_data(CURL *handle, char *location_key, char *api_key);

#endif
