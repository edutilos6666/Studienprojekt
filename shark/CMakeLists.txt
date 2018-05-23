# # See docs/CMake.html for instructions about how to build shark with CMake.
cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

# Assemble shark version number
if(NOT DEFINED SHARK_VERSION_MAJOR)
  	set(SHARK_VERSION_MAJOR 0)
endif()
if(NOT DEFINED SHARK_VERSION_MINOR)
  	set(SHARK_VERSION_MINOR 0)
endif()
if(NOT DEFINED SHARK_VERSION_PATCH)
  	set(SHARK_VERSION_PATCH 1)
endif()

# Initialize shark project with specific program version
project(shark VERSION ${SHARK_VERSION_MAJOR}.${SHARK_VERSION_MINOR}.${SHARK_VERSION_PATCH})


# Define build type (use 'Release' as default)
if (NOT CMAKE_BUILD_TYPE)
	message("[+] Build Type: 'Release' (default).")
  	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
else()
	message("[+] Build Type: '" ${CMAKE_BUILD_TYPE} "'.")
endif()


# Enable CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	message("[+] CCache (enabled).")
 	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
	message("[+] CCache (disabled).")
endif(CCACHE_FOUND)

# Build LLVM for MSP430 target architecture 
set(LLVM_TARGETS_TO_BUILD "MSP430" CACHE STRING "build target MSP430" FORCE)
# Disable build of LLVM examples
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "disable build of LLVM examples" FORCE)
# Disable build of LLVM tests
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "disable build of LLVM tests" FORCE)
# Disable build of LLVM tools 
set(LLVM_INCLUDE_TOOLS OFF CACHE BOOL "disable build of LLVM tools" FORCE)
set(LLVM_BUILD_TOOLS OFF CACHE BOOL "disable build of LLVM tools" FORCE)

# Add custom LLVM directory
add_subdirectory("llvm")

# Assemble LLVM library components for linking to shark framework
llvm_map_components_to_libnames(llvm_libs core irreader bitwriter)

# Output executables to bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Output libraries to lib folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build shark framework
add_subdirectory("src")
