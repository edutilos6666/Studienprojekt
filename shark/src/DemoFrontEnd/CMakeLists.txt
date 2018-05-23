# Set demoFrontEnd include 
set(DEMOFrontEND_INC
    
)

# Set demoFrontEnd sources 
set(DEMOFrontEND_SRC 
    DemoFrontEnd.cpp
)

# Assemble demoFrontEnd executable 
add_executable(demoFrontEnd ${DEMOFrontEND_SRC} ${DEMOFrontEND_INC})

# Define include directories 
target_include_directories(demoFrontEnd PRIVATE 
	${PROJECT_SOURCE_DIR}/llvm/include 
	${PROJECT_BINARY_DIR}/llvm/include
)

# Link demoFrontEnd framework to LLVM
target_link_libraries(demoFrontEnd ${llvm_libs})