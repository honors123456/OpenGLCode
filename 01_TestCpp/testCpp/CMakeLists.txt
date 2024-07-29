cmake_minimum_required(VERSION 3.8)

project(TestCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8 /execution-charset:utf-8")
endif()


set(HEADERS )
set(SOURCES "src/main.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})