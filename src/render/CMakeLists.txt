set(target lib-render)

add_library(${target} STATIC
	stb_image_impl.cpp
	pipeline.cpp
	render.cpp
)

if(WIN32)
    target_link_libraries(${target}
        PRIVATE
        glfw3
        glew32
        opengl32
	#assimp-vc143-mtd
	#lib-mobs
        lib-playable
    )
else()
    target_link_libraries(${target}
        PRIVATE
        glfw
        GL
        GLEW
        #assimp
	#lib-mobs
        lib-playable
    )
endif()
