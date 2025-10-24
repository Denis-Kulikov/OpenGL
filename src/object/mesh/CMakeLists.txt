#add_subdirectory(primitive)
#add_subdirectory(loader)
#add_subdirectory(skeleton)

set(target lib-mesh)

add_library(${target} STATIC
	mesh.cpp
	skeletal_mesh.cpp
	loader/assimp_mesh_loader.cpp
	loader/tinygltf_mesh_loader.cpp
	primitive/sprite.cpp
	primitive/cube.cpp
	skeleton/skeleton.cpp
	skeleton/animation/animator.cpp
	skeleton/animation/skeletal_animation.cpp
)

target_link_libraries(${target}
    PRIVATE	
	assimp-vc143-mtd
)
