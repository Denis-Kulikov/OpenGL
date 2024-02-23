set(target lib-object)

add_library(${target} STATIC
	objectTransform.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-math
)
