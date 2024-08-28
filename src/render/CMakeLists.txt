set(target lib-render)

add_library(${target} STATIC
	stb_image_impl.cpp
	free_type.cpp
)

if(WIN32)
    target_link_libraries(${target}
        PRIVATE
        glfw3
        glew32
        opengl32
	freetype
    )
else()
    find_package(Freetype REQUIRED)
    
    target_include_directories(${target} PRIVATE ${FREETYPE_INCLUDE_DIRS})
    
    target_link_libraries(${target}
        PRIVATE
        glfw
        GL
        GLEW
	freetype
        ${FREETYPE_LIBRARIES}
    )
endif()