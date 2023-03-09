cmake_minimum_required(VERSION 3.2)
project( simpleRequest CXX )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 14 )

add_executable( simpleRequest ${CMAKE_CURRENT_SOURCE_DIR}/simpleRequest.cpp ) 
target_include_directories( simpleRequest PUBLIC ${CMAKE_SOURCE_DIR}/include )

set( CURL_DIR ${CMAKE_SOURCE_DIR}/../curl-7.88.1_2-win64-mingw )

find_library( HTTPSREQUEST NAMES httpsRequests HINTS ${CMAKE_SOURCE_DIR}/build )
find_library( CURL NAMES libcurl PATHS ${CURL_DIR} )

target_link_libraries( simpleRequest ${HTTPSREQUEST} )
target_link_libraries( simpleRequest ${CURL} )

target_include_directories( simpleRequest PUBLIC ${CURL_DIR}/include )