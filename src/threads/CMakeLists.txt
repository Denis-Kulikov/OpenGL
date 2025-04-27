set(target lib-threads)

add_library(${target} STATIC
	thread_pool.cpp
)

