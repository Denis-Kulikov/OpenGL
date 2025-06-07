set(target lib-component-template)

add_library(${target} STATIC
	camera.cpp
	mesh.cpp
	shape.cpp
	static_mehs.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-component
	lib-geometry
)
