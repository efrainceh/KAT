cmake_minimum_required(VERSION 3.20.1)

set(This KAT)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Sources
	src/textIndex.cpp
	src/genomeParser.cpp
	src/genome.cpp
	src/referenceAligner.cpp
	src/fileFolders.cpp
	src/inputOutput.cpp
)

set(Headers
	include/textIndex.h
	include/genomeParser.h	
	include/genome.h
	include/referenceAligner.h
	include/fileFolders.h
	include/inputOutput.h
)

add_library(${This} STATIC ${Sources} ${Headers})

target_include_directories(${This} PRIVATE ${include})

add_subdirectory(test)