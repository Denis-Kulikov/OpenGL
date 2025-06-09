set(target lib-component-template)

add_library(${target} STATIC
	camera.cpp
	shape.cpp
	mesh.cpp
	skeletal_mesh.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-component
	lib-geometry
)
