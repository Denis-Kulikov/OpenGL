set(target lib-glfw)

add_library(${target} STATIC
	glfw.cpp
	pipeline.cpp
	render.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	glfw GL GLEW
	lib-object
	lib-components
	lib-playable
	lib-mobs
)
