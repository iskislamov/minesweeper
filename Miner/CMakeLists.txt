cmake_minimum_required(VERSION 3.16)

project("Miner")
	
AUX_SOURCE_DIRECTORY(. source)
 
add_library(Miner ${source})

target_include_directories(Miner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})