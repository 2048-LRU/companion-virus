cmake_minimum_required(VERSION 3.31)
project(companion_virus VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

# Add subdirectories with refactored structure
# src/CMakeLists.txt will manage: core/ ui/ companion/
add_subdirectory(src)

set(CPACK_PACKAGE_NAME "companion")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Companion Virus")
set(CPACK_GENERATOR "TGZ")

include(CPack)
