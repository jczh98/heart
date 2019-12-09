cmake_minimum_required(VERSION 3.14)
project(chihaya)

set(CMAKE_CXX_STANDARD 17)

# glog
add_subdirectory(external/glog)
add_subdirectory(external/fmt)
include_directories(external/stb)
include_directories(external/glog/src)

file(GLOB coreSrc
        src/core/*.cc
        src/core/*.h)

set(CHIHAYA_LIBS
        glog
        fmt)

add_executable(chihaya src/main.cpp
        ${coreSrc})

target_link_libraries(chihaya ${CHIHAYA_LIBS})