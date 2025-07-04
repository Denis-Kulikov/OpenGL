add_subdirectory(primitive)
add_subdirectory(animation)

set(target lib-geometry)

add_library(${target} STATIC
	primitive.cpp
	mesh.cpp
	skeletal_mesh.cpp
)

target_link_libraries(${target}
    PRIVATE	
	assimp-vc143-mtd
	lib-animation
)
