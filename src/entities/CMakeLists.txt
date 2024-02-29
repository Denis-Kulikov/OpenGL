set(target lib-entities)

add_library(${target} STATIC
	component.cpp
	entities.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	#lib-entities
)
