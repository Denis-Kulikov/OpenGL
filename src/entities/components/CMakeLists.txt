set(target lib-components)

add_library(${target} STATIC
	bone.cpp
	components.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-object
	pugixml
)
