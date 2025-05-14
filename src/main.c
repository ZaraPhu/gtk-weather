#include <gtk/gtk.h>

#define CSS_FILE "style.css"

static void load_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GFile *css_file = g_file_new_for_path(CSS_FILE);
    gtk_css_provider_load_from_file(provider, css_file);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), 1);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Weather");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    GtkWidget *label = gtk_label_new("Weather Application");
    gtk_widget_add_css_class(label, "inside-text");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(box), label);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **args) {
    GtkApplication *app = gtk_application_new("com.example.gtk-weather", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "startup", G_CALLBACK(load_css), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, args);
    g_object_unref(app);
    return status;
}
