cmake_minimum_required( VERSION 3.2 )
project( httpsRequests CXX )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 14 )

file( GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp )

add_library( httpsRequests STATIC ${sources} )

target_include_directories( httpsRequests PUBLIC ${CMAKE_SOURCE_DIR}/include )

find_library( CURL NAMES libcurl_a         HINTS ${CMAKE_SOURCE_DIR}/lib/ )
set( HTTPSREQUESTS_DEPENDENCIES ${CURL} )

target_link_libraries( httpsRequests PUBLIC ${CURL} )

add_subdirectory( examples )