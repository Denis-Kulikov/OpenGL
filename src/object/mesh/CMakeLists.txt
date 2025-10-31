set(target lib-mesh)

add_library(${target} STATIC
	mesh_data.cpp
	mesh.cpp
	skeletal_mesh.cpp
	loader/assimp_mesh_loader.cpp
	#loader/tinygltf_mesh_loader.cpp
	primitive/sprite.cpp
	primitive/cube.cpp
	skeleton/skeleton.cpp
	skeleton/animation/animator.cpp
	skeleton/animation/skeletal_animation.cpp
)

target_link_libraries(${target}
    PRIVATE	
	lib-material
	assimp-vc143-mtd
)
