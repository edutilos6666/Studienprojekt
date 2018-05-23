add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(demopass MODULE DemoPass.cpp)

# Define include directories 
target_include_directories(demopass PRIVATE 
	${PROJECT_SOURCE_DIR}/llvm/include 
	${PROJECT_BINARY_DIR}/llvm/include
)

# workaround for Error opening 'DemoPass/libdemopass.so': DemoPass/libdemopass.so: undefined symbol: _ZTIN4llvm12FunctionPassE
# source: https://stackoverflow.com/questions/17225956/developing-an-llvm-pass-with-cmake-out-of-llvm-source-directory
SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")

# Link demopass to LLVM
# removed to prevent this error:
# opt: CommandLine Error: Option 'debug-pass' registered more than once!
# LLVM ERROR: inconsistency in registered CommandLine options
# do not link modules with LLVM, it is already present in the binary it will be loaded in later
#target_link_libraries(demopass ${llvm_libs})

# standalone version of demopass, can run without opt
add_executable(demopass_standalone DemoPassStandalone.cpp)
# Define include directories 
target_include_directories(demopass_standalone PRIVATE 
	${PROJECT_SOURCE_DIR}/llvm/include 
	${PROJECT_BINARY_DIR}/llvm/include
)
target_link_libraries(demopass_standalone ${llvm_libs})