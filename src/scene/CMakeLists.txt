set(target lib-scene)

add_library(${target} STATIC
	camera.cpp
	scene.cpp
	light/scene_lighting.cpp
	shadow/scene_shadow.cpp
	shadow/shadow_point_light.cpp
	shadow/shadow_directional_light.cpp
)

target_link_libraries(${target}
    PRIVATE	
	lib-manager
)
