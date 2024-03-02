set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
	#component.cpp
	#entities.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	pugixml
)
