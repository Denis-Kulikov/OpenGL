set(target lib-decor)

add_library(${target} STATIC
	grass.cpp
	wooden_box.cpp
	skybox.cpp
	brick_sphere.cpp
	stone_floor.cpp
	tree.cpp
	test_dq.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
	lib-component-template
	lib-component-template-dual-quat
)
