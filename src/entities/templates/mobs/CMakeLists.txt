set(target lib-mobs)

add_library(${target} STATIC
	Female.cpp
  dual_quat_skining.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-component-template
)
