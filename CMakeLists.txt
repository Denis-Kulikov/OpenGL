cmake_minimum_required(VERSION 3.20)

project(openGL VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)
if(WIN32)
	include_directories(assimp/include)
	include_directories(build/_deps/bullet-src/src)
	link_directories(build/_deps/bullet-src/build/lib/Debug)
	include_directories("C:/Program Files/LLVM/include")
else()
	include_directories(/usr/include/bullet)
endif()

include_directories(build/_deps/stb_image-src)
include_directories(build/_deps/glm-src)

link_directories(lib)
add_subdirectory(src)
add_subdirectory(external)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)