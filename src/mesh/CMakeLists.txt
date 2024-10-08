set(target lib-mesh)

add_library(${target} STATIC
	#mesh.cpp
	custom_mesh.cpp
	primitive_mesh.cpp
)

find_package(OpenMP REQUIRED)

target_link_libraries(${target}
    PRIVATE
	lib-render
    	lib-data
    	OpenMP::OpenMP_CXX
)

