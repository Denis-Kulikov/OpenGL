set(target lib-mobs)

add_library(${target} STATIC
	spider.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
)
