cmake_minimum_required(VERSION 4.0)
project(GTKWeather LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(LIBCURL REQUIRED IMPORTED_TARGET libcurl)

add_executable(gtk_weather src/main.c src/utils.c)
set_target_properties(gtk_weather PROPERTIES CXX_STANDARD 23)

target_link_libraries(gtk_weather PRIVATE PkgConfig::GTK4)
target_link_libraries(gtk_weather PRIVATE PkgConfig::LIBCURL)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(src/style.css style.css COPYONLY)
