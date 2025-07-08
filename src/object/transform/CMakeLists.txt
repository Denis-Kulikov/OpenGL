set(target lib-transform)

add_library(${target} STATIC
	transform.cpp
	rigid_transform.cpp
	transform_dual_quat.cpp
)

if(WIN32)
target_link_libraries(${target}
    PRIVATE
	BulletCollision_Debug
	BulletDynamics_Debug
	LinearMath_Debug
)
endif()
