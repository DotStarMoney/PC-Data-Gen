cmake_minimum_required(VERSION 3.2)
project(PCDataGen)


set(HDR_FILES 
	
	body.h
	context.h
	gfxfunctions.h
	image.h
	noncopyable.h
	parameter.h
	SDL_stbimage.h
	term.h
	vec2.h
	parameter_ro.h
	color32.h
	utility.h
	
	PARENT_SCOPE)

source_group("Header Files" FILES ${HDR_FILES})