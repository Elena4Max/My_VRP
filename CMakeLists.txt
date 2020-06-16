cmake_minimum_required(VERSION 2.8) 

project(test)				
find_package( Threads )
set(SOURCE_EXE main.cpp)			

add_executable(main ${SOURCE_EXE})	

target_link_libraries(main jsoncpp ${CMAKE_THREAD_LIBS_INIT})

