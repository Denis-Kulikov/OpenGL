set(target lib-mobs)

add_library(${target} STATIC
	spider.cpp
	bullet.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object	
	lib-components
)
