cmake_minimum_required (VERSION 2.6)
project (Metasim)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -DFILE_OFFSET_BITS=64") 
set(CMAKE_C_FLAGS_DEBUG  "-Wall -DFILE_OFFSET_BITS=64") 

add_subdirectory (tools)
add_subdirectory (src)
add_subdirectory (src/VirtualFS)

