cmake_minimum_required(VERSION 3.0)
project(EffectivePacketFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SPDLOG_PATH ${CMAKE_CURRENT_LIST_DIR}/submodules/spdlog)
set(SPD_LOG_INCLUDE_DIRECTORY ${SPDLOG_PATH}/include/)
add_subdirectory(${SPDLOG_PATH})

set(CATCH2_PATH ${CMAKE_CURRENT_LIST_DIR}/submodules/Catch2)
set(CATCH2_INCLUDE_DIRECTORY ${CATCH2_PATH}/submodules/Catch2/src)
add_subdirectory(${CATCH2_PATH})

set (ROOT_INCLUDE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/.)
set (AGENT_INCLUDE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/agent)

add_subdirectory(./agent)
add_subdirectory(./tests/agent)

