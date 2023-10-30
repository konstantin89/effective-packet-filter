cmake_minimum_required(VERSION 3.0)
project(EffectivePacketFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SPDLOG_PATH ${CMAKE_CURRENT_LIST_DIR}/submodules/spdlog)
add_subdirectory(${SPDLOG_PATH})

set (SPD_LOG_INCLUDE_DIRECTORY ${SPDLOG_PATH}/include/)
set (COMMON_INCLUDE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/.)

add_subdirectory(./agent)
