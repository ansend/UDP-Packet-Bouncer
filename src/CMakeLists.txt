project(UDP_Packt_Bouncer)
cmake_minimum_required(VERSION 2.8.1)

#add cmake module to be able to find glib (liglib2)
set(CMAKE_MODULE_PATH
${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#find glib
FIND_PACKAGE(GLIB)

#definition of common header and source files
set (COMMON_HEADERS Meteron_Link_Test_Common.h posix_periodic.h Sender_Class.h Receiver_Class.h)
set (COMMON_SOURCES posix_periodic.cpp Sender_Class.cpp Receiver_Class.cpp)

#external includes
include_directories(${GLIB_INCLUDE_DIRS})

#executable for the boncer
set (BOUNCER_HEADERS ${COMMON_HEADERS} ASAP_Bouncer_Main.h)
set (BOUNCER_SOURCES ${COMMON_SOURCES} ASAP_Bouncer_Main.cpp)
add_executable(Bouncer ${BOUNCER_HEADERS} ${BOUNCER_SOURCES})
target_link_libraries(Bouncer ${GLIB_LIBRARIES} pthread)


#executable for the sender
set (SENDER_HEADERS ${COMMON_HEADERS} Measurement_Sender_Main.h)
set (SENDER_SOURCES ${COMMON_SOURCES} Measurement_Sender_Main.cpp)
add_executable(Sender ${SENDER_HEADERS} ${SENDER_SOURCES})
target_link_libraries(Sender ${GLIB_LIBRARIES} pthread)
