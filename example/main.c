#include <gio/gio.h>
#include <glib.h>
#include <time.h>

#include "joe_dbus.h"

static GMainLoop *main_loop = NULL;
static GDBusConnection *dbus_connection = NULL;
static JoeMessager *messager = NULL;
static JoeCounter *counter = NULL;

int main(int argc, char *argv[]) {

  g_autoptr(GError) error = NULL;

  main_loop = g_main_loop_new(NULL, FALSE);
  if (!main_loop)
    return EXIT_FAILURE;

  GDBusConnection *dbus_connection =
      g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
  if (error) {
    g_error("Cannot get system bus: %s", error->message);
    return EXIT_FAILURE;
  }

  messager = joe_messager_skeleton_new();
  counter = joe_counter_skeleton_new();

  g_bus_own_name_on_connection(dbus_connection, "org.gnome.Joe",
                               G_BUS_NAME_OWNER_FLAGS_NONE, NULL, NULL, NULL,
                               NULL);

  g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(messager),
                                   dbus_connection, "/org/gnome/Joe", &error);
  if (error) {
    g_error("Export dbus interface failed: %s", error->message);
    return EXIT_FAILURE;
  }

  g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(counter),
                                   dbus_connection, "/org/gnome/Joe", &error);
  if (error) {
    g_error("Export dbus interface failed: %s", error->message);
    return EXIT_FAILURE;
  }

  // g_signal_connect(message, "handle-set-message", GCallback(on_handle_set_message), NULL);

  return 0;
}
