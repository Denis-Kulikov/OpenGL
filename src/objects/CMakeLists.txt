set(target lib-object)

add_library(${target} STATIC
	objectTransform.cpp
	sprite.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-math
)
