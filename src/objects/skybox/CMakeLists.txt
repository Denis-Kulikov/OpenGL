set(target lib-skybox)

add_library(${target} STATIC
	skybox.cpp
)

target_link_libraries(${target}
    PRIVATE
)
