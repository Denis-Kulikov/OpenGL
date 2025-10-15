set(target lib-interface)

add_library(${target} STATIC
	component.cpp

)

target_link_libraries(${target}
    PRIVATE
	lib-manager
)
