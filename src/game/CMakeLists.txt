set(target lib-game)

add_library(${target} STATIC
	gameManager.cpp
	bulletManager.cpp
)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIRS})


target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	
	BulletCollision
	BulletDynamics
	LinearMath
)
