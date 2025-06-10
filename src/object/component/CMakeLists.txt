set(target lib-component)

add_subdirectory(template)

add_library(${target} STATIC
	component.cpp
	component_physics.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-transform
	lib-material
	lib-primitive
)
