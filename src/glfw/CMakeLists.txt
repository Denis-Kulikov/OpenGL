set(target lib-glfw)

add_library(${target} STATIC
	glfw.cpp
	pipeline.cpp
	render.cpp
)

target_link_libraries(
  ${target}
	#glfw GL GLEW
#lib/glfw3
#lib/glew32
	#C:/msys64/mingw64/lib/libglfw3.a C:/msys64/mingw64/lib/libglew32.a C:/msys64/mingw64/lib/libopengl32.a
    #C:/prog/OpenGL/lib/glfw3.lib
    #C:/prog/OpenGL/lib/glew32.lib
    #opengl32

	lib-entities
	lib-threads
	lib-playable
	lib-mobs
	lib-decor
)
