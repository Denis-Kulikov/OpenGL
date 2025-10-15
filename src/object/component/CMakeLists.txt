set(target lib-component)

add_library(${target} STATIC
	component.cpp
	component_physics.cpp
	component_dual_quat.cpp
	template/camera.cpp
	template/mesh.cpp
	template/skeletal_mesh.cpp
	template/skeletal_matrix_mesh.cpp
	template/skeletal_dq_mesh.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-transform
	lib-material
	lib-mesh
)
