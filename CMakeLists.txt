
cmake_minimum_required(VERSION 2.8) 

project(test)				
find_package( Threads )

set(SOURCES Vehicle.cpp global.cpp display.cpp init.h)
set(HEADERS Vehicle.h global.h display.h init.cpp)
add_library(point Point.cpp Point.h)
add_library(act Act.cpp Act.h)
add_library(routes Routes.cpp Routes.h)
add_library(shipment Shipment.cpp Shipment.h)
add_library(lib ${SOURCES} ${HEADERS})

set(SOURCE_EXE main.cpp)			
add_definitions( -D_BONUS1 )
add_executable(main ${SOURCE_EXE})	

target_link_libraries(main jsoncpp lib point act routes shipment ${CMAKE_THREAD_LIBS_INIT})

