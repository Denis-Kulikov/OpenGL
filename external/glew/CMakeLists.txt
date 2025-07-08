include(FetchContent)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/nigels-com/glew
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(glew)