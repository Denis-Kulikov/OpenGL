include(FetchContent)

FetchContent_Declare(
    boostorg_statechart
    https://github.com/boostorg/statechart
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(boostorg_statechart)
