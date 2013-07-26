include_directories (${Metasim_SOURCE_DIR}/src/VirtualFS)
link_directories (${Metasim_SOURCE_DIR}/src/VirtualFS)
add_executable(map2bindex map2bindex.cpp)
target_link_libraries(map2bindex Index)
