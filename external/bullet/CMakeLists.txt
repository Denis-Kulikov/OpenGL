include(FetchContent)

FetchContent_Declare(
    bullet
    GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
    GIT_TAG 3.25
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

set(BUILD_EXTRAS OFF CACHE INTERNAL "")
set(BUILD_BULLET3 OFF CACHE INTERNAL "")
set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "")
set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "")
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "")

cmake_policy(SET CMP0115 NEW)

FetchContent_MakeAvailable(bullet)
