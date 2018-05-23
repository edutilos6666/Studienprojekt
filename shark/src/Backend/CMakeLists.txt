# Set demoBackEnd include 
set(BACKEND_INC
    
)

# Set demoBackEnd sources 
set(BACKEND_SRC
		Backend.cpp
)

# Assemble demoBackEnd executable 
add_executable(backend ${BACKEND_SRC} ${BACKEND_INC})

# Define include directories 
target_include_directories(backend PRIVATE
	${PROJECT_SOURCE_DIR}/llvm/include 
	${PROJECT_BINARY_DIR}/llvm/include
)

# Link demoBackEnd framework to LLVM
target_link_libraries(backend ${llvm_libs})