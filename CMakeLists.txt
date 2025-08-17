cmake_minimum_required(VERSION 3.16)
project(BasicGeant4Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Geant4_DIR for custom installation
if(NOT DEFINED Geant4_DIR)
    set(Geant4_DIR "/Users/morbo/CLionProjects/libraries/geant4/geant4_v11/geant4_v11_install/lib/cmake/Geant4")
endif()

# Set MacPorts paths for dependencies
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/local")
endif()

# Find required packages
find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(Threads REQUIRED)

# Include Geant4 headers
include(${Geant4_USE_FILE})

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh)

# Add executable
add_executable(basicTest main.cc ${sources} ${headers})

# Set output directory for executable
set_target_properties(basicTest PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/executable
)

# Copy macro files to the executable directory
configure_file(${CMAKE_SOURCE_DIR}/init_vis.mac ${CMAKE_CURRENT_SOURCE_DIR}/executable/init_vis.mac COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/gui.mac ${CMAKE_CURRENT_SOURCE_DIR}/executable/gui.mac COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/init_vis.mac ${CMAKE_CURRENT_SOURCE_DIR}/executable/run.mac COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/gui.mac ${CMAKE_CURRENT_SOURCE_DIR}/executable/run_mt.mac COPYONLY)

# Link libraries
target_link_libraries(basicTest ${Geant4_LIBRARIES} Threads::Threads)

# Optimization flags for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(basicTest PRIVATE -O3 -DNDEBUG)
endif()