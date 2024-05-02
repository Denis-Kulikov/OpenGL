set(target lib-playable)

add_library(${target} STATIC
	Wilson.cpp
	unit.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object	
	lib-components
)
