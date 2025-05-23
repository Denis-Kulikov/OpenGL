set(target lib-mesh)

add_library(${target} STATIC
	mesh.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-render
)