set(target lib-decor)

add_library(${target} STATIC
	wooden_box.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
	lib-component-template
)
