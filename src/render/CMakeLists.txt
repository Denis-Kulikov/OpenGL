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
	assimp-vc143-mtd
	pugixml
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
	pugixml
        lib-threads
        lib-playable
        lib-mobs
        lib-decor
    )
endif()