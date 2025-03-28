cmake_minimum_required(VERSION 3.8)

project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

if(MSVC)
	if(MSVC_TOOLSET_VERSION EQUAL 141)
		message("current msvc toolset is msvc-v${MSVC_TOOLSET_VERSION}")
	else()
		set(MSVC_TOOLSET_VERSION 141)
	endif()
	add_compile_options(/Zc:__cplusplus)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8 /execution-charset:utf-8")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(OPENGL_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)

set(HEADERS "")
set(SOURCES "main.cpp")

aux_source_directory(./src DIR_SRCS)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME}
					${OPENGL_LIB_DIR}/glfw3.lib
					${OPENGL_LIB_DIR}/OpenGL32.Lib)