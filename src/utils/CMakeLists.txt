# Look for gtk4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_executable(converter converter.c)

# Include GTK
target_include_directories(converter PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(converter PRIVATE ${GTK_CFLAGS_OTHER})
target_link_libraries(converter PRIVATE ${GTK_LIBRARIES} m)