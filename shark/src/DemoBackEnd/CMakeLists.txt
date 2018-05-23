# Set demoBackEnd include 
set(DEMOBACKEND_INC
    
)

# Set demoBackEnd sources 
set(DEMOBACKEND_SRC 
    DemoBackEnd.cpp
)

# Assemble demoBackEnd executable 
add_executable(demoBackEnd ${DEMOBACKEND_SRC} ${DEMOBACKEND_INC})

# Define include directories 
target_include_directories(demoBackEnd PRIVATE 
	${PROJECT_SOURCE_DIR}/llvm/include 
	${PROJECT_BINARY_DIR}/llvm/include
)

# Link demoBackEnd framework to LLVM
target_link_libraries(demoBackEnd ${llvm_libs})