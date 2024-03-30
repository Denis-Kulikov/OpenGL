set(target lib-decor)

add_library(${target} STATIC
	wave.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object	
	lib-components
)
