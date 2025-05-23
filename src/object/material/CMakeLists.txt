set(target lib-material)

add_library(${target} STATIC
	shader.cpp
	material.cpp
	texture.cpp
)
