cmake_minimum_required(VERSION 3.0)

set(SOURCES main.cpp)
set(AGENT_APP agent)

add_executable(${AGENT_APP} ${SOURCES})

set(AGENT_INCLUDE_PATHS     
    ${SPD_LOG_INCLUDE_DIRECTORY}   
    ${CHAIN_OF_RESPONSIBILITY_PATH_INCLUDE_PATH}  
    ${ROOT_INCLUDE_DIRECTORY}
    ${CMAKE_CURRENT_LIST_DIR})  
 
add_subdirectory(infra)
add_subdirectory(processing_chain)

target_include_directories(${AGENT_APP} PRIVATE ${AGENT_INCLUDE_PATHS})

target_link_libraries(${AGENT_APP} agent_infra_lib spdlog::spdlog)
