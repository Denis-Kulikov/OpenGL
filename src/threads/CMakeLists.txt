set(target lib-threads)

add_library(${target} STATIC
	threads.cpp
)


if(WIN32)
    target_link_libraries(${target}
        PRIVATE	
	${CMAKE_THREAD_LIBS_INIT}
	lib-entities
    )
else()
    target_link_libraries(${target}
        PRIVATE
	pthread
	lib-entities
    )
endif()