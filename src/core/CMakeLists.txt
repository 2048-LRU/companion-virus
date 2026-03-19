find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

find_package(OpenSSL REQUIRED)
pkg_check_modules(GD REQUIRED gdlib)

add_library(companion_core OBJECT
    base_converter.c
    caesar_encrypt.c
    file_hasher.c
    temp_converter.c
    num_generator.c
    img_inverter.c
    mediaplayer_core.c
)

target_include_directories(companion_core PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GLIB_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${GD_INCLUDE_DIRS}
)

target_compile_options(companion_core PRIVATE ${GLIB_CFLAGS_OTHER})
target_link_libraries(companion_core PRIVATE
    ${GLIB_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${GD_LIBRARIES}
    m
)
