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

    set(TEST_EXE "${util}_test")
    add_executable(${TEST_EXE} ${PROJECT_SOURCE_DIR}/test/utils/${TEST_EXE}.c ${util}/core.c)
    target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/include ${GLIB_INCLUDE_DIRS} ${GD_INCLUDE_DIRS})
    target_link_libraries(${TEST_EXE} PRIVATE ${GLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${GD_LIBRARIES} m)
    target_compile_options(${TEST_EXE} PRIVATE ${GLIB_CFLAGS_OTHER})

    add_test(NAME ${util} COMMAND ${TEST_EXE})
endforeach()
