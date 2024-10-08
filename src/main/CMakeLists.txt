set(target openGL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${target})

target_sources(
  ${target}
  PRIVATE
    main.cpp
)

find_package(OpenMP REQUIRED)

target_link_libraries(
  ${target}
  PRIVATE
	lib-render
	lib-game
	lib-data
	OpenMP::OpenMP_CXX
)
