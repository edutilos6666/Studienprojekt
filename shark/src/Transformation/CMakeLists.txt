add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_executable(transformation transformation.cpp)


# Define include directories
target_include_directories(transformation PRIVATE
	${PROJECT_SOURCE_DIR}/llvm/include
	${PROJECT_BINARY_DIR}/llvm/include
)

target_link_libraries(transformation ${llvm_libs})

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")
