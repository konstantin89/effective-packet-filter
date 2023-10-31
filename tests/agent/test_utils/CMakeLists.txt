cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCES "*.cpp")

add_library(${AGENT_TEST_UTILS_LIB_NAME} STATIC ${SOURCES})

target_include_directories(${AGENT_TEST_UTILS_LIB_NAME} PRIVATE ${ROOT_INCLUDE_DIRECTORY})

# target_link_libraries(${AGENT_TEST_UTILS})
