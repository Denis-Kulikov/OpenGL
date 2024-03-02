include(FetchContent)

FetchContent_Declare(
    pigixml
    GIT_REPOSITORY https://github.com/zeux/pugixml
    GIT_TAG v1.14
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(pigixml)
