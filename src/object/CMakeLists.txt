set(target lib-object)

add_subdirectory(component)
add_subdirectory(skybox)
add_subdirectory(shader)
add_subdirectory(transform)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
	line.cpp
	cube_wire.cpp
	cube.cpp
)

target_link_libraries(${target}
    PRIVATE
	#lib-skybox
	lib-shader
)
