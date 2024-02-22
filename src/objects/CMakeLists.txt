set(target lib-object)

add_library(${target} STATIC
	ObjectTransform.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-math
)
