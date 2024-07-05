include(FetchContent)

FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/planetack/stb_image
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(stb_image)
