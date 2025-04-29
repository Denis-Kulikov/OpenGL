set(target lib-dynamic-compiler)

find_package(Clang REQUIRED CONFIG)
include_directories(SYSTEM ${CLANG_INCLUDE_DIRS})

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})
include(DetermineGCCCompatible)

add_definitions(${LLVM_DEFINITIONS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs
	Core
	ExecutionEngine
	OrcJIT
	RuntimeDyld
	Support
	native
)

if(LLVM_COMPILER_IS_GCC_COMPATIBLE)
  if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  endif()
  if(NOT LLVM_ENABLE_EH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
  endif()
endif()

add_library(${target} STATIC dynamic_compiler.cpp)
include_directories(${CMAKE_SOURCE_DIR})
target_link_libraries(${target} 
	${llvm_libs}
	clangAST
	clangBasic
	clangLex
)