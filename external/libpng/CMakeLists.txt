include(FetchContent)

FetchContent_Declare(
    libpng
    https://github.com/pnggroup/libpng.git
    GIT_SHALLOW TRUE
    GIT_TAG v1.7.0beta89
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(libpng)
