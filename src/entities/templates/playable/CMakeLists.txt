set(target lib-playable)

add_library(${target} STATIC
	Ghost.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
)
