set(target lib-decor)

add_library(${target} STATIC

)

target_link_libraries(
  ${target}
  PRIVATE
	lib-entities
)
