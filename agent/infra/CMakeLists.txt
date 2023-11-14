cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCES "*.cpp")

set(AGENT_INFRA_LIB agent_infra_lib)

add_library(${AGENT_INFRA_LIB} STATIC ${SOURCES})

target_include_directories(${AGENT_INFRA_LIB} PRIVATE ${AGENT_INCLUDE_PATHS})

target_link_libraries(${AGENT_INFRA_LIB} spdlog::spdlog)
