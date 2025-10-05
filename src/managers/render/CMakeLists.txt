set(target lib-render-manager)

add_library(${target} STATIC
	buffer_manager.cpp
)


target_link_libraries(
  ${target}
  PRIVATE
	lib-render
)
