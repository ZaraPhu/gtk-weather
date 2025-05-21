/*
The purpose of this file is to serve as a header file to export functions
from the utils.c code to the main.c code.
Author: Zara Phukan.
Creation Date: May 14, 2025.
*/

/*** Dependencies ***/
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <stddef.h>

#ifndef UTILS_H
#define UTILS_H

/*** Enums/Structs Definition ***/
enum ForecastFrequency {
    ONE_HOUR,
    TWELVE_HOURS,
    TWENTY_FOUR_HOURS,
    SEVENTY_TWO_HOURS,
    ONE_TWENTY_HOURS,
    ONE_DAY,
    FIVE_DAYS,
    TEN_DAYS,
    FIFTEEN_DAYS,
};

typedef struct {
    char *location_key;
    char *api_key;
} WeatherDataRequestParameters;

/*** Function Declarations ***/
int load_dotenv(char *file_path);
void load_css(char *css_file_path);
size_t write_data(char *buffer, size_t len, size_t nmemb, void *user_data);
int request_weather_data(char *location_key, char *api_key);

#endif
