cmake_minimum_required(VERSION 3.16)

project("Miner")

include_directories(".")

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory("Miner")

SET(OBJS ${CMAKE_CURRENT_SOURCE_DIR}//mine.o)

add_executable(miner ${OBJS} main.cpp)

target_link_libraries(miner LINK_PUBLIC Miner)

set_property(TARGET miner PROPERTY CXX_STANDARD 11)

SET_SOURCE_FILES_PROPERTIES(
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)