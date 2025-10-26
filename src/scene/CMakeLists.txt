set(target lib-scene)

add_library(${target} STATIC
	scene.cpp
	light/scene_lighting.cpp
	shadow/cube_map.cpp
	shadow/scene_shadow.cpp
)

target_link_libraries(${target}
    PRIVATE	
	lib-manager
)
