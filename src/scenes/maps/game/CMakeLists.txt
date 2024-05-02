set(target lib-main-map)

add_library(${target} STATIC
	main_map.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	glfw GL GLEW
	lib-object
	lib-components
	lib-playable
	lib-mobs
	lib-decor
)
