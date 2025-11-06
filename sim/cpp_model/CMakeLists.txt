cmake_minimum_required(VERSION 3.10)
project(HeteroAISimulator VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files for simulator library
set(SIM_SOURCES
    src/common_types.cpp
    src/vector_core.cpp
    src/tensor_core.cpp
    src/scheduler.cpp
    src/memory.cpp
    src/interconnect.cpp
)

# Create simulator library
add_library(sim_core STATIC ${SIM_SOURCES})

# Main simulator executable
add_executable(simulator src/main.cpp)
target_link_libraries(simulator sim_core pthread)

# Test executable
add_executable(sim_test src/test.cpp)
target_link_libraries(sim_test sim_core pthread)

# Installation
install(TARGETS simulator sim_test DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/hetero_ai_sim)

# Print configuration
message(STATUS "========================================")
message(STATUS "Hetero AI Processor Simulator - Configuration")
message(STATUS "========================================")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler:    ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard:    ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Flags:       ${CMAKE_CXX_FLAGS}")
message(STATUS "Install prefix:  ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")
