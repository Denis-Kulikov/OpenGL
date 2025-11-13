set(target lib-ui)

add_library(${target} STATIC
	visitor/component/title.cpp
	visitor/component/controller.cpp
)

target_link_libraries(${target}
    PRIVATE
	lib-component
	imgui
)
