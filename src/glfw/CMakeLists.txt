set(target lib-glfw)

add_library(${target} STATIC
	distance.cpp
	glfw.cpp
	pipeline.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	glfw GL GLEW
	stb_image
	lib-math
	lib-object
)
