set(target lib-mobs)

add_library(${target} STATIC
	Female.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-mobs
)
