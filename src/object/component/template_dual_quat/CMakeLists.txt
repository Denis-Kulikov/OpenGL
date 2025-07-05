set(target lib-component-template-dual-quat)

add_library(${target} STATIC
	shape.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-component
	lib-geometry
)
