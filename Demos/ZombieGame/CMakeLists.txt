cmake_minimum_required(VERSION 2.8.7)
project("ZombieGame")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++14")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
    add_definitions(-DEFGE_USE_STD_RANDOMENGINE)
endif()

set(SOURCES
  MainGame.cpp
  Agent.cpp
  Bullet.cpp
  main.cpp
  Gun.cpp
  Human.cpp
  Level.cpp
  Player.cpp
  Zombie.cpp)

add_executable(ZombieGame ${SOURCES})

find_package(OpenGL REQUIRED)

find_library(GangerEngine
    NAMES GangerEngine
    PATHS ${LIBRARY_DIR}
    NO_DEFAULT_PATH
)

find_library(SDL2
    NAMES SDL2
    PATHS ${LIBRARY_DIR}/SDL
    NO_DEFAULT_PATH
)

find_library(SDL2_MIXER
    NAMES SDL2_mixer
    PATHS ${LIBRARY_DIR}/SDL
    NO_DEFAULT_PATH
)

find_library(SDL2_TTF
    NAMES SDL2_ttf
    PATHS ${LIBRARY_DIR}/SDL
    NO_DEFAULT_PATH
)

find_library(SDL2MAIN
    NAMES SDL2main
    PATHS ${LIBRARY_DIR}/SDL
    NO_DEFAULT_PATH
)

find_library(CEGUIBASE_DEBUG
    NAMES CEGUIBase-0_d
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(CEGUIEXPATPARSER_DEBUG
    NAMES CEGUIExpatParser_d
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(CEGUIOPENGLRENDERER_DEBUG
    NAMES CEGUIOpenGLRenderer-0_d
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(CEGUIBASE
    NAMES CEGUIBase-0
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(CEGUIEXPATPARSER
    NAMES CEGUIExpatParser
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(CEGUIOPENGLRENDERER
    NAMES CEGUIOpenGLRenderer-0
    PATHS ${LIBRARY_DIR}/CEGUI
    NO_DEFAULT_PATH
)

find_library(GLEW
    NAMES glew32
    PATHS ${LIBRARY_DIR}/glew
    NO_DEFAULT_PATH
)

#Bring the headers into the project
include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/../../deps/include
  ${CMAKE_SOURCE_DIR}/../../include
  ${CMAKE_SOURCE_DIR})

link_directories(
  ${CMAKE_SOURCE_DIR}/../../deps/lib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/CEGUI
  ${CMAKE_SOURCE_DIR}/../../deps/lib/freetype
  ${CMAKE_SOURCE_DIR}/../../deps/lib/glew
  ${CMAKE_SOURCE_DIR}/../../deps/lib/SDL)

target_link_libraries(ZombieGame
  ${OPENGL_LIBRARIES}
  ${GangerEngine}
  ${SDL2} ${SDL2MAIN} ${SDL2_MIXER} ${SDL2_TTF}
  debug ${CEGUIBASE_DEBUG} optimized ${CEGUIBASE}
  debug ${CEGUIEXPATPARSER_DEBUG} optimized ${CEGUIEXPATPARSER}
  debug ${CEGUIOPENGLRENDERER_DEBUG} optimized ${CEGUIOPENGLRENDERER}
  ${GLEW}
)
