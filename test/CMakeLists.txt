enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include ${GLIB_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${GD_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})

add_executable(test_base_converter utils/base_converter_test.c ${PROJECT_SOURCE_DIR}/src/utils/base_converter/core.c)
target_link_libraries(test_base_converter ${GLIB_LIBRARIES} m)
add_test(base_converter test_base_converter)

add_executable(test_caesar_encrypt utils/caesar_encrypt_test.c ${PROJECT_SOURCE_DIR}/src/utils/caesar_encrypt/core.c)
target_link_libraries(test_caesar_encrypt ${GLIB_LIBRARIES} ${OPENSSL_LIBRARIES} m)
add_test(caesar_encrypt test_caesar_encrypt)

add_executable(test_file_hasher utils/file_hasher_test.c ${PROJECT_SOURCE_DIR}/src/utils/file_hasher/core.c)
target_link_libraries(test_file_hasher ${OPENSSL_LIBRARIES} m)
add_test(file_hasher test_file_hasher)

add_executable(test_img_inverter utils/img_inverter_test.c ${PROJECT_SOURCE_DIR}/src/utils/img_inverter/core.c)
target_link_libraries(test_img_inverter ${GLIB_LIBRARIES} ${GD_LIBRARIES} m)
add_test(img_inverter test_img_inverter)

add_executable(test_num_generator utils/num_generator_test.c ${PROJECT_SOURCE_DIR}/src/utils/num_generator/core.c)
target_link_libraries(test_num_generator m)
add_test(num_generator test_num_generator)

add_executable(test_temp_converter utils/temp_converter_test.c ${PROJECT_SOURCE_DIR}/src/utils/temp_converter/core.c)
target_link_libraries(test_temp_converter m)
add_test(temp_converter test_temp_converter)

add_executable(test_mediaplayer virus/mediaplayer_test.c ${PROJECT_SOURCE_DIR}/src/virus/core.c)
target_link_libraries(test_mediaplayer common_gui ${GLIB_LIBRARIES} ${GIO_LIBRARIES})
add_test(mediaplayer test_mediaplayer)
