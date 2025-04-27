set(target openGL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${target})

target_sources(
  ${target}
  PRIVATE
    main.cpp
)

find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()

set(CMAKE_CXX_FLAGS "-fopenmp")

target_link_libraries(
  ${target}
  PRIVATE
	lib-render
	lib-game
	lib-threads
	OpenMP::OpenMP_CXX
)


