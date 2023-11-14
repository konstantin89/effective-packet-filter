cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCES "*.cpp")

set(PROCESSING_CHAIN_LIB processing_chain_lib)

add_library(${PROCESSING_CHAIN_LIB} STATIC ${SOURCES})

target_include_directories(${PROCESSING_CHAIN_LIB} PRIVATE ${AGENT_INCLUDE_PATHS})

target_link_libraries(${PROCESSING_CHAIN_LIB} spdlog::spdlog)
