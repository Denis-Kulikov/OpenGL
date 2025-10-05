set(target lib-mobs)

add_library(${target} STATIC
#	female.cpp
  dual_quat_skining.cpp
  matrix_skining.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-component-template
)
