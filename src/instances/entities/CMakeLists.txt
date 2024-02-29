set(target lib-entities)

add_library(${target} STATIC
	
)

target_link_libraries(
  ${target}
  PRIVATE
	#lib-entities
)
