cmake_minimum_required(VERSION 3.12.4)
project(emga VERSION 0.1.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(CTest)
enable_testing()

add_subdirectory(lvgl)
add_executable(emga main.c)
add_compile_options(-std=gnu11)

# Specify path to own LVGL config header
set(LV_CONF_PATH
${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
CACHE STRING "" FORCE)

# The target "MyFirmware" depends on LVGL
target_link_libraries(emga PRIVATE lvgl)
target_link_libraries(emga PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)