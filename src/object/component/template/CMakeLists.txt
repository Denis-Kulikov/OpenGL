set(target lib-component-template)

add_library(${target} STATIC
	camera.cpp
	mesh.cpp
	skeletal_mesh.cpp
	skeletal_matrix_mesh.cpp
	skeletal_dq_mesh.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-component
	lib-mesh
)
