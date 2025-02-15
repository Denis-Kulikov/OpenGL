set(target lib-game)

add_library(${target} STATIC
	gameManager.cpp
	#bulletManager.cpp
)

if(WIN32)
target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	BulletCollision_Debug
	BulletDynamics_Debug
	LinearMath_Debug
)
else()
target_link_libraries(${target} PRIVATE lib-object)
endif()
