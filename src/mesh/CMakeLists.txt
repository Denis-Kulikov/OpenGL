set(target lib-mesh)

add_library(${target} STATIC
	mesh.cpp
	custom_mesh.cpp
	shape.cpp
	physics_shape.cpp
)


target_link_libraries(${target}
    PRIVATE
    lib-render
)

