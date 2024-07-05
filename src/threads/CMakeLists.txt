set(target lib-threads)

add_library(${target} STATIC
	threads.cpp
)

target_link_libraries(
  ${target}
  PRIVATE
	${CMAKE_THREAD_LIBS_INIT}
	lib-entities
	lib-glfw
)
if(WIN32)
    target_link_libraries(${target}
        PRIVATE
	lib-entities
	lib-glfw
    )
else()
    target_link_libraries(${target}
        PRIVATE
	pthread
	lib-entities
	lib-glfw
    )
endif()