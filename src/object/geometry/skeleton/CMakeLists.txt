add_subdirectory(animation)

set(target lib-skeleton)

add_library(${target} STATIC
	skeleton.cpp
)

target_link_libraries(${target}
    PRIVATE	
	lib-animation
)
