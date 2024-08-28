include(FetchContent)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype
    GIT_SHALLOW TRUE
    GIT_TAG VER-2-12-1
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(freetype)
