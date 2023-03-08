cmake_minimum_required(VERSION 3.0)
project(simpleRequest CXX)

add_executable( simpleRequest ${CMAKE_CURRENT_SOURCE_DIR}/simpleRequest.cpp ) 
target_include_directories( simpleRequest PUBLIC ${CMAKE_SOURCE_DIR}/include )

find_library( HTTPSREQUEST NAMES libhttpsRequests HINTS ${CMAKE_SOURCE_DIR}/build )

target_link_libraries( simpleRequest PUBLIC ${HTTPSREQUEST} )
target_link_libraries( simpleRequest PUBLIC ${HTTPSREQUESTS_DEPENDENCIES} )