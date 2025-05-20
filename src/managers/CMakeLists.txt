set(target lib-manager)

add_library(${target} STATIC
	global_state.cpp
	render_manager.cpp
	time_manager.cpp
	window_manager.cpp
	#bulletManager.cpp
)

if(WIN32)
target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	lib-render
	#BulletCollision_Debug
	#BulletDynamics_Debug
	#LinearMath_Debug
)
else()
target_link_libraries(${target} PRIVATE lib-object)
endif()
