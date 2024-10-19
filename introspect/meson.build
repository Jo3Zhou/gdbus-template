gnome = import('gnome')

gio_dep = dependency('gio-2.0')

gdbus_codegen = gnome.gdbus_codegen('joe_dbus',
  sources: 'org.gnome.Joe.xml',
  interface_prefix : 'org.gnome.Joe.',
  namespace: 'Joe',
)

generated_sources = gdbus_codegen[0]
generated_headers = gdbus_codegen[1]

lib_joe_dbus = static_library('lib_joe_dbus',
  generated_sources,
  include_directories: include_directories('.'),
  dependencies: gio_dep,
  install: true
)

lib_joe_dbus_dep = declare_dependency(
  link_with: lib_joe_dbus,
  include_directories: include_directories('.'),
  dependencies: gio_dep
)
