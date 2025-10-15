set(target lib-material)

add_library(${target} STATIC
	shader.cpp
	material.cpp
	texture.cpp
	texture_unit.cpp
)

target_link_libraries(${target}
    PRIVATE	
	assimp-vc143-mtd
)