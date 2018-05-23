# Set Frontend include 
set(Frontend_INC
    
)

# Set demoFrontEnd sources 
set(Frontend_SRC 
    Frontend.cpp
)

# Assemble demoFrontEnd executable 
add_executable(Frontend ${Frontend_SRC} ${Frontend_INC})

# Define include directories 
target_include_directories(Frontend PRIVATE 
        ${PROJECT_SOURCE_DIR}/llvm/include 
        ${PROJECT_BINARY_DIR}/llvm/include
)

# Link demoFrontEnd framework to LLVM
target_link_libraries(Frontend ${llvm_libs})
