cmake_minimum_required(VERSION 3.0)

set(AGENT_TEST_UTILS_LIB_NAME agent_tests_utils)

add_subdirectory(./test_utils)

file(GLOB INFRA_SOURCES "infra/*.cpp")
set(TEST_AGENT_APP tests)

add_executable(${TEST_AGENT_APP} ${INFRA_SOURCES})


set(AGENT_TESTS_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(${TEST_AGENT_APP} PRIVATE 
    ${SPD_LOG_INCLUDE_DIRECTORY} 
    ${ROOT_INCLUDE_DIRECTORY}
    ${AGENT_INCLUDE_DIRECTORY}
    ${CATCH2_INCLUDE_DIRECTORY}
    ${AGENT_TESTS_ROOT_DIRECTORY})


target_link_libraries(${TEST_AGENT_APP}  
    Catch2 
    Catch2WithMain  
    ${AGENT_TEST_UTILS_LIB_NAME}
    agent_infra_lib
    spdlog::spdlog)
