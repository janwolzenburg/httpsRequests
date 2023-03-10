cmake_minimum_required( VERSION 3.2 )
project( httpsRequests CXX )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 14 )

file( GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/include/*.h)
add_library( httpsRequests STATIC ${sources} )
target_include_directories( httpsRequests PUBLIC ${CMAKE_SOURCE_DIR}/include )

# Modify!
set( CURL_DIR ${CMAKE_SOURCE_DIR}/../curl-7.88.1_2-win64-mingw )

target_include_directories( httpsRequests PUBLIC ${CURL_DIR}/include )
find_library( CURL NAMES libcurl PATHS ${CURL_DIR} )
target_link_libraries( httpsRequests ${CURL} )


add_executable( simpleRequest ${CMAKE_SOURCE_DIR}/examples/simpleRequest.cpp )
target_include_directories( simpleRequest PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories( simpleRequest PUBLIC ${CURL_DIR}/include )
target_link_libraries( simpleRequest httpsRequests )
target_link_libraries( simpleRequest ${CURL} )

add_executable( jsonRequest ${CMAKE_SOURCE_DIR}/examples/jsonRequest.cpp )
target_include_directories( jsonRequest PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories( jsonRequest PUBLIC ${CURL_DIR}/include )
target_link_libraries( jsonRequest httpsRequests )
target_link_libraries( jsonRequest ${CURL} )