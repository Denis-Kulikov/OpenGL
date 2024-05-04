set(target server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${target})

target_sources(
  ${target}
  PRIVATE
    server.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-glfw
	lib-object
	lib-game
	#OpenMP::OpenMP_CXX
)
