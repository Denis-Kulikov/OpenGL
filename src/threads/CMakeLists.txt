set(target lib-threads)

add_library(${target} STATIC
	threads.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	glfw GL GLEW
	lib-entities
)
