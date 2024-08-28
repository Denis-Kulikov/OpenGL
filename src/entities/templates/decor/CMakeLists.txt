set(target lib-decor)

add_library(${target} STATIC
	wave.cpp
	grass.cpp
	symbol.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
)
