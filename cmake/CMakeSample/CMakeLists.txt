PROJECT(cmakesample)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules 
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

ADD_SUBDIRECTORY(src)

