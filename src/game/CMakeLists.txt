set(target lib-game)

add_library(${target} STATIC
	gameManager.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	freetype
	lib-render
)
