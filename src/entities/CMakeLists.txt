set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
	pawn.cpp
	character.cpp
	#entities.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	pugixml
	lib-components
)

add_subdirectory(components)