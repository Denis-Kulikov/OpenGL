set(target lib-glfw)

add_library(${target} STATIC
	glfw.cpp
	pipeline.cpp
	render.cpp
)

if(WIN32)
    target_link_libraries(${target}
        PRIVATE
        glfw3
        glew32
        opengl32
	assimp-vc143-mtd
	#lib-model
        lib-entities
        lib-threads
        lib-playable
        lib-mobs
        lib-decor
    )
else()
    target_link_libraries(${target}
        PRIVATE
        glfw
        GL
        GLEW
        lib-entities
        lib-threads
        lib-playable
        lib-mobs
        lib-decor
    )
endif()