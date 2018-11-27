CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(examples)

MESSAGE("*****************************************************************")
MESSAGE(${PROJECT_NAME})
MESSAGE("*****************************************************************")

SET(CMAKE_CXX_FLAGS "-std=c++11")

INCLUDE_DIRECTORIES(
	../include
	../include/StdPlus
)

ADD_EXECUTABLE(${PROJECT_NAME} msvc/src/main.cpp)