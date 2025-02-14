set(target lib-mesh)

add_library(${target} STATIC
	mesh.cpp
	shape.cpp
	physics_shape.cpp
)

find_package(OpenMP REQUIRED)

target_link_libraries(${target}
    PRIVATE
	lib-render
)

