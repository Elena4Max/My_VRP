cmake_minimum_required(VERSION 2.8) 

project(test)				
find_package( Threads )

set(SOURCES Vehicle.cpp global.cpp Point.cpp Act.cpp Routes.cpp Shipment.cpp display.cpp init.h)
set(HEADERS Vehicle.h global.h Point.h Act.h Routes.h Shipment.h display.h init.cpp)
add_library(lib ${SOURCES} ${HEADERS})

set(SOURCE_EXE main.cpp)			
add_definitions( -D_BONUS1 )
add_executable(main ${SOURCE_EXE})	

target_link_libraries(main jsoncpp lib ${CMAKE_THREAD_LIBS_INIT})

