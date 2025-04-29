set(target lib-game)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_library(${target} STATIC
	gameManager.cpp
	#bulletManager.cpp
)

if(WIN32)
target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	#BulletCollision_Debug
	#BulletDynamics_Debug
	#LinearMath_Debug
)
else()
target_link_libraries(${target} PRIVATE lib-object)
endif()
