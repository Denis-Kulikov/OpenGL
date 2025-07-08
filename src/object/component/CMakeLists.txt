set(target lib-component)

add_subdirectory(template)
add_subdirectory(template_dual_quat)

add_library(${target} STATIC
	component.cpp
	component_physics.cpp
	component_dual_quat.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-transform
	lib-material
	lib-primitive
)
