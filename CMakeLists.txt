cmake_minimum_required(VERSION 3.3.2)
project("Ganger Engine")

# Set the library type to Shared
SET (LIB_TYPE SHARED)

#Bring the headers into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_library(GangerEngine ${SOURCES})
