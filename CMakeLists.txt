cmake_minimum_required(VERSION 3.18.4)

project(openGL VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")

link_directories(lib)

include_directories(include)
include_directories(assimp/include)

include_directories(build/_deps/stb_image-src)
include_directories(build/_deps/pugixml-src/src)
include_directories(build/_deps/glm-src)


add_subdirectory(src)
add_subdirectory(external)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(DEFINED CMAKE_LINKER_FLAGS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
endif()

