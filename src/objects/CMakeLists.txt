set(target lib-object)

add_subdirectory(skybox)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
	line.cpp
	cube_bone.cpp
	cube.cpp
)

