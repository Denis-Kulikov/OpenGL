set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
	templates/decor/skybox.cpp
	templates/decor/house.cpp
	templates/decor/stone_floor.cpp
	templates/decor/wooden_box.cpp
	templates/mobs/female.cpp
  	templates/mobs/dual_quat_skining.cpp
  	templates/mobs/matrix_skining.cpp
	templates/playable/Ghost.cpp
)

target_link_libraries(
  ${target}
  PRIVATE	
	lib-component
)
