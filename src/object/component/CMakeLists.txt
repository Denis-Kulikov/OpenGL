set(target lib-component)

add_subdirectory(template)

add_library(${target} STATIC
	component.cpp
	#rigid.cpp
	#transform.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-transform
	lib-material
	lib-primitive
	#lib-mesh
)
