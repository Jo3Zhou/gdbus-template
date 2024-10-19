find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED IMPORTED_TARGET gio-unix-2.0)

set(SOURCES main.c)

add_executable(gdbus-example1 ${SOURCES})

target_link_libraries(gdbus-example1
  PkgConfig::GLIB
  PkgConfig::GIO
  PkgConfig::GIO_UNIX

  lib-joe-dbus
)
