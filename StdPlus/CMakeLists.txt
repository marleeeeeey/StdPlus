CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(StdPlus)

MESSAGE("*****************************************************************")
MESSAGE(${PROJECT_NAME})
MESSAGE("*****************************************************************")

SET(CMAKE_CXX_FLAGS "-std=c++11")

ADD_DEFINITIONS(-DSTD_PLUS_EXPORT)
# ADD_DEFINITIONS(-DSTD_PLUS_DLL) #Uncomment if need dll 

INCLUDE_DIRECTORIES(
	./include
	./include/StdPlus
)

ADD_LIBRARY(${PROJECT_NAME} SHARED ./src/StdPlus/Ones.cpp)