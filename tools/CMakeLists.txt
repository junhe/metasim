
include_directories (${Metasim_SOURCE_DIR}/src/VirtualFS)
link_directories (${Metasim_SOURCE_DIR}/src/VirtualFS)

include_directories (${Metasim_SOURCE_DIR}/src)
link_directories (${Metasim_SOURCE_DIR}/src)

add_executable(map2bindex map2bindex.cpp)
target_link_libraries(map2bindex Index Util MapFetcher)

add_executable(mergewrites mergewrites.cpp)
target_link_libraries(mergewrites Index Util MapFetcher)

add_executable(insertglobal_driver insertglobal_driver.cpp)
target_link_libraries(insertglobal_driver Index Util MapFetcher)

