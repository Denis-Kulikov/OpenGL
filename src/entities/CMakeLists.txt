add_subdirectory(templates)

set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
	pawn.cpp
	character.cpp
	owner.cpp
)

target_link_libraries(
  ${target}
  PRIVATE	
	lib-mesh
	lib-component
)
