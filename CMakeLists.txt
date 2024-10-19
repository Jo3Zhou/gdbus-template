cmake_minimum_required(VERSION 3.28)

project(gdbus-template
  DESCRIPTION "A gdbus-codegen template and usage project"
)

enable_language(C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(introspect)
add_subdirectory(example)
