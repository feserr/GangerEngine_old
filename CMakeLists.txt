cmake_minimum_required(VERSION 2.8.8)
project("Ganger Engine")

# Set the library type to Shared
SET (LIB_TYPE SHARED)

#Bring the headers into the project
include_directories(libs)
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_library(GangerEngine ${SOURCES})
