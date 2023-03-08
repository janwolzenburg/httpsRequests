cmake_minimum_required(VERSION 3.0)
#project(httpsRequests CXX)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

ADD_LIBRARY( httpsRequests STATIC ${sources} )

target_include_directories(httpsRequests PUBLIC ${CMAKE_SOURCE_DIR}/include )

find_library(CURL NAMES libcurl HINTS ${CMAKE_SOURCE_DIR}/lib/win64)
find_library(SSL NAMES libssl ./lib/win64 HINTS ${CMAKE_SOURCE_DIR}/lib/win64)
find_library(CRYPTO NAMES libcrypto ./lib/win64 HINTS ${CMAKE_SOURCE_DIR}/lib/win64)

#target_link_libraries(diveraWin64 PUBLIC ${CURL} ${SSL} ${CRYPTO})