# Set shark include
set(SHARK_INC

)

# Set shark sources
set(SHARK_SRC
    main.cpp
)

include_directories(
${PROJECT_SOURCE_DIR}/include
)

# Assemble shark executable
add_executable(shark ${SHARK_SRC} ${SHARK_INC})

# Define include directories
target_include_directories(shark PRIVATE
	${PROJECT_SOURCE_DIR}/llvm/include
	${PROJECT_BINARY_DIR}/llvm/include
)

# Link shark framework to LLVM
target_link_libraries(shark ${llvm_libs})

# build the Demo frontend
add_subdirectory("DemoFrontEnd")

# build the Demo pass
add_subdirectory("DemoPass")

# build the Demo backend
add_subdirectory("DemoBackEnd")

# build backend
add_subdirectory("Backend")

# build transformation
add_subdirectory("Transformation")

# build frontend
add_subdirectory("Frontend")
