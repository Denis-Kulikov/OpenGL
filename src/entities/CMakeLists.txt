add_subdirectory(components)
add_subdirectory(templates)

set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
	pawn.cpp
	character.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object	
	lib-components
)
