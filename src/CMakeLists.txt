include_directories ( ${Metasim_SOURCE_DIR}/src )
include_directories ( ${Metasim_SOURCE_DIR}/src/VirtualFS )

add_library (Util Util.cpp)
add_library (MapFetcher MapFetcher.cpp)
