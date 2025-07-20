add_subdirectory(primitive)
add_subdirectory(loader)
add_subdirectory(skeleton)

set(target lib-geometry)

add_library(${target} STATIC
	primitive.cpp
	mesh.cpp
	skeletal_mesh.cpp
)

target_link_libraries(${target}
    PRIVATE	
	lib-mesh-loader
	lib-skeleton
)
