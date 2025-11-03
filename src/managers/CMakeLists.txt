set(target lib-manager)

add_library(${target} STATIC
	global.cpp
	time.cpp
	bullet.cpp
	render/buffer.cpp
	render/pipeline.cpp
	render/render.cpp
	render/stb_image_impl.cpp
	window/window.cpp
	window/window_manager.cpp
	window/imgui.cpp
)

if(WIN32)
target_link_libraries(
  ${target}
  PRIVATE
	glfw
	glew32
	opengl32
	lib-entities
	BulletCollision_Debug
	BulletDynamics_Debug
	LinearMath_Debug
)
else()
target_link_libraries(${target} PRIVATE
	glfw
	GL
	GLEW
	assimp
	lib-entities
	lib-object
)
endif()
