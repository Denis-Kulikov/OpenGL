add_subdirectory(templates)

set(target lib-entities)

add_library(${target} STATIC
	actor.cpp
)

target_link_libraries(
  ${target}
  PRIVATE	
	lib-component
)
