#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>

#include "joe_dbus.h"

static GMainLoop *main_loop = NULL;
static GDBusConnection *dbus_connection = NULL;
static JoeMessager *messager = NULL;
static JoeCounter *counter = NULL;

static gboolean handle_set_message(JoeMessager *object,
                                   GDBusMethodInvocation *invocation,
                                   const gchar *arg_Message, gpointer notused) {

  /* Step1: deprecate old message */
  joe_messager_emit_deprecated_message(object,
                                       joe_messager_get_message(object));

  /* Step2: set new message */
  joe_messager_set_message(object, arg_Message);

  /* Step3: increment counter */
  joe_counter_set_count(counter, joe_counter_get_count(counter) + 1);

  /* Step4: finish the call */
  joe_messager_complete_set_message(object, invocation);

  return G_DBUS_METHOD_INVOCATION_HANDLED;
}

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

  g_signal_connect(messager, "handle-set-message",
                   G_CALLBACK(handle_set_message), NULL);

  g_main_loop_run(main_loop);

  g_object_unref(main_loop);
  g_object_unref(messager);
  g_object_unref(counter);

  return 0;
}
