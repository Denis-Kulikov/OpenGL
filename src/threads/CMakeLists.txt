set(target lib-threads)

add_library(${target} STATIC
	threads.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
	lib-glfw
)
