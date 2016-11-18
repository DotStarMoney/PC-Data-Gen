cmake_minimum_required(VERSION 3.2)
project(PCDataGen)

set(EX_PROJ_SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/Source)
set(EX_PROJ_BUILD_DIR ${CMAKE_BINARY_DIR}/deps/Build)

configure_file(deps-CMakeLists.txt deps/CMakeLists.txt COPYONLY)
execute_process(COMMAND ${CMAKE_COMMAND} . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps)

include_directories(${EX_PROJ_SOURCE_DIR}/SDL_EX/include
                    ${EX_PROJ_SOURCE_DIR}/STB_IMAGE_EX
					${EX_PROJ_SOURCE_DIR}/GLM_EX/glm)

add_subdirectory(${CMAKE_BINARY_DIR}/../src)
add_subdirectory(${CMAKE_BINARY_DIR}/../inc)

set(ABSOLUTE_SRC_FILES "")
set(ABSOLUTE_HDR_FILES "")
foreach(filen ${SRC_FILES})
	list(APPEND ABSOLUTE_SRC_FILES "src/${filen}")
endforeach(filen)
foreach(filen ${HDR_FILES})
	list(APPEND ABSOLUTE_HDR_FILES "inc/${filen}")
endforeach(filen)
include_directories(${CMAKE_BINARY_DIR}/../inc)


include_directories(${CMAKE_BINARY_DIR}/inc)
add_executable(PCDataGen ${ABSOLUTE_SRC_FILES} ${ABSOLUTE_HDR_FILES})

if(WIN32)
    target_link_libraries(PCDataGen debug ${EX_PROJ_BUILD_DIR}/SDL_EX/Debug/SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
    target_link_libraries(PCDataGen debug ${EX_PROJ_BUILD_DIR}/SDL_EX/Debug/SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX})
	target_link_libraries(PCDataGen optimized ${EX_PROJ_BUILD_DIR}/SDL_EX/Release/SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
	target_link_libraries(PCDataGen optimized ${EX_PROJ_BUILD_DIR}/SDL_EX/Release/SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX})
	target_link_libraries(PCDataGen winmm.lib imm32.lib version.lib)
endif()
if(UNIX)
	#
	#
endif()