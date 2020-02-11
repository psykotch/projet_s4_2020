#include <gtk/gtk.h>

// Signal handler for the "clicked" signal of the start button.
void on_start(GtkButton *button, gpointer user_data)
{
    g_print("on_start()\n");
}


// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "plain.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), NULL);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
