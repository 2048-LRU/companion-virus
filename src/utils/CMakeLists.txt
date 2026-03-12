# Look for gtk4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_executable(base_converter base_converter.c)
add_executable(cypher cypher.c)

# Include GTK
target_include_directories(base_converter PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(base_converter PRIVATE ${GTK_CFLAGS_OTHER})
target_link_libraries(base_converter PRIVATE ${GTK_LIBRARIES} m)

target_include_directories(cypher PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(cypher PRIVATE ${GTK_CFLAGS_OTHER})
target_link_libraries(cypher PRIVATE ${GTK_LIBRARIES})