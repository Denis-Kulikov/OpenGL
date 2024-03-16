set(target lib-game)

add_library(${target} STATIC
	gameManager.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	glfw GL GLEW
	lib-object
)
