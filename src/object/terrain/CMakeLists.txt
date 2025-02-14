set(target lib-terrain)

add_library(${target} STATIC
	terrain.cpp
)

target_link_libraries(${target}
    PRIVATE
)
