set(target openGL)

add_executable(${target})

target_sources(
  ${target}
  PRIVATE
    main.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	lib-glfw
	lib-math
	lib-object
)
