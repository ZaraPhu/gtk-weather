#include <gtk/gtk.h>
#include <curl/curl.h>
#include <math.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSS_FILE "style.css"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define DIAGONAL sqrt(pow(WINDOW_HEIGHT, 2) + pow(WINDOW_HEIGHT, 2))

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Weather");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    GtkWidget *label = gtk_label_new("Weather Application");
    gtk_widget_add_css_class(label, "blue-theme");
    if (DIAGONAL < 300) {
        gtk_widget_add_css_class(label, "title-small");
    } else if (DIAGONAL < 800) {
        gtk_widget_add_css_class(label, "title-medium");
    } else if (DIAGONAL < 1200) {
        gtk_widget_add_css_class(label, "title-large");
    } else {
        gtk_widget_add_css_class(label, "title-xlarge");
    }

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(box), label);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **args) {
    if (load_dotenv("../.env") == EXIT_FAILURE) { return EXIT_FAILURE; }
    const char *accuweather_api_key = getenv("ACCUWEATHER_API_KEY");
    if (accuweather_api_key == NULL) { fprintf(stderr, "Could not load AccuWeather API key"); }

    CURL *curl_handle = curl_easy_init();

    GtkApplication *app = gtk_application_new("com.example.gtk-weather", G_APPLICATION_DEFAULT_FLAGS);
    if (app == NULL) { fprintf(stderr, "Could not start Gtk application.\n"); return EXIT_FAILURE; }
    g_signal_connect_swapped(app, "startup", G_CALLBACK(load_css), CSS_FILE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, args);

    g_object_unref(app);
    return status;
}
