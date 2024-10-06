set(target lib-object)

add_subdirectory(skybox)
add_subdirectory(shader)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
	line.cpp
	cube.cpp
	cube_wire.cpp
	cube_simple.cpp
)

target_link_libraries(${target}
    PRIVATE
	#lib-skybox
	lib-shader
)
