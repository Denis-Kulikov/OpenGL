set(target lib-transform)

add_library(${target} STATIC
	collider_transform.cpp
	rigid_transform.cpp
	transform.cpp
)

target_link_libraries(${target}
    PRIVATE
	BulletCollision_Debug
	BulletDynamics_Debug
	LinearMath_Debug
)
