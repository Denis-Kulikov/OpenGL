set(target lib-object)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
	sphere.cpp
	line.cpp
	cube_bone.cpp
#	circle.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-math
)
