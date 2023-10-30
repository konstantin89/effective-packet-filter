cmake_minimum_required(VERSION 3.0)

set(SOURCES main.cpp infra/File.cpp)

set(APP_NAME agent)

add_executable(${APP_NAME} ${SOURCES})

target_include_directories(${APP_NAME} PRIVATE ${SPD_LOG_INCLUDE_DIRECTORY} ${COMMON_INCLUDE_DIRECTORY})

target_link_libraries(${APP_NAME} spdlog::spdlog)
