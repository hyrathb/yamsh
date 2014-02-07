#include <stdlib.h>
#include <gtk/gtk.h>
#include "methods.h"

int main (int argc, char *argv[])
{
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    create_main();
    create_room_win();
    create_guest_win();

    gtk_main ();
    return 0;
}
