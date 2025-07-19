set(target lib-mesh-loader)

add_library(${target} STATIC
	assimp_mesh_loader.cpp
	tinygltf_mesh_loader.cpp
)

target_link_libraries(${target}
    PRIVATE	
	assimp-vc143-mtd
	lib-animation
)
