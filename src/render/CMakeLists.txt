set(target lib-render)

add_library(${target} STATIC
	stb_image_impl.cpp
	pipeline.cpp
)


if(WIN32)
    target_link_libraries(${target}
        PRIVATE
        glfw
        glew32
        opengl32
	lib-decor
	lib-mobs
        lib-playable
    )
else()
    target_link_libraries(${target}
        PRIVATE
        glfw
        GL
        GLEW
        assimp
	lib-decor
	lib-mobs
        lib-playable
    )
endif()
