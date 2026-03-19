set(UTILS
    base_converter
    caesar_encrypt
    file_hasher
    img_inverter
    num_generator
    temp_converter
)

foreach(util ${UTILS})
    add_executable(${util} ${util}/core.c ${util}/ui.c)
    target_include_directories(${util} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${util} PRIVATE common_gui ${GLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${GD_LIBRARIES} m)
    target_compile_options(${util} PRIVATE ${GLIB_CFLAGS_OTHER})
endforeach()
