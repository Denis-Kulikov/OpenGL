set(target lib-object)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
	line.cpp
	cube_bone.cpp
)

