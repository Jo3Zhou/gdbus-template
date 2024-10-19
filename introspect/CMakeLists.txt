find_package(PkgConfig REQUIRED)

find_program(GDBUS_CODEGEN_EXECUTABLE NAMES gdbus-codegen)
if(NOT GDBUS_CODEGEN_EXECUTABLE)
    message(FATAL_ERROR "gdbus-codegen not found")
endif()

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED IMPORTED_TARGET gio-unix-2.0)

# Command to run gdbus-codegen
add_custom_command(
    PRE_BUILD
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/joe_dbus.c
           ${CMAKE_CURRENT_BINARY_DIR}/joe_dbus.h
    COMMAND gdbus-codegen
    ARGS --interface-prefix org.gnome.Joe
         --generate-c-code joe_dbus
         --c-namespace Joe
         --c-generate-object-manager
         --output-directory ${CMAKE_CURRENT_BINARY_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/org.gnome.Joe.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running gdbus-codegen from xml"
)

add_library(lib-joe-dbus OBJECT
    ${CMAKE_CURRENT_BINARY_DIR}/joe_dbus.c
    ${CMAKE_CURRENT_BINARY_DIR}/joe_dbus.h
)

target_include_directories(lib-joe-dbus PUBLIC
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(lib-joe-dbus
    PkgConfig::GLIB
    PkgConfig::GIO
    PkgConfig::GIO_UNIX
)
