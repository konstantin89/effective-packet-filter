cmake_minimum_required(VERSION 3.0)

add_subdirectory(infra)

set(SOURCES main.cpp)
set(AGENT_APP agent)

add_executable(${AGENT_APP} ${SOURCES})

target_include_directories(${AGENT_APP} PRIVATE ${SPD_LOG_INCLUDE_DIRECTORY} ${ROOT_INCLUDE_DIRECTORY})

target_link_libraries(${AGENT_APP} agent_infra_lib spdlog::spdlog)
