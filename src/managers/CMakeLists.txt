set(target lib-manager)

add_library(${target} STATIC
	global.cpp
	time.cpp
	bullet.cpp
	render/render.cpp
	render/buffer.cpp
	window/window.cpp
	window/imgui.cpp
)

if(WIN32)
target_link_libraries(
  ${target}
  PRIVATE
	lib-render
	BulletCollision_Debug
	BulletDynamics_Debug
	LinearMath_Debug
)
else()
target_link_libraries(${target} PRIVATE lib-object)
endif()
