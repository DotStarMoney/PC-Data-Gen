cmake_minimum_required(VERSION 3.2)
project(PCDataGen)


set(SRC_FILES 
	
	main.cpp 
	context.cpp
	image.cpp
	gfxfunctions.cpp
	utility.cpp
		
	PARENT_SCOPE)

source_group("Source Files" FILES ${SRC_FILES})