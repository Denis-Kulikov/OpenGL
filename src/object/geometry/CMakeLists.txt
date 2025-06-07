add_subdirectory(primitive)

set(target lib-geometry)

add_library(${target} STATIC
	mesh.cpp
	primitive.cpp
)

target_link_libraries(${target}
    PRIVATE	
	assimp-vc143-mtd
)
