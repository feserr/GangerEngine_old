cmake_minimum_required(VERSION 2.8.7)
project("NinjaPlatformer")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++14")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
    add_definitions(-DEFGE_USE_STD_RANDOMENGINE)
endif()

set(SOURCES
  App.cpp
  Box.cpp
  Capsule.cpp
  EditorScreen.cpp
  GameplayScreen.cpp
  LevelReaderWriter.cpp
  main.cpp
  MainMenuScreen.cpp
  Player.cpp)

add_executable(NinjaPlatformer ${SOURCES})

find_package(OpenGL REQUIRED)

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
  ${CMAKE_SOURCE_DIR}/../../deps/lib/SDL
  ${CMAKE_SOURCE_DIR}/../../deps/lib/Box2D)

target_link_libraries(NinjaPlatformer
  ${OPENGL_LIBRARIES}
  ${CMAKE_SOURCE_DIR}/../../deps/lib/libGangerEngine.a
  ${CMAKE_SOURCE_DIR}/../../deps/lib/SDL/libSDL2-2.0.0.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/SDL/libSDL2_mixer-2.0.0.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/SDL/libSDL2_ttf-2.0.0.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/CEGUI/libCEGUIBase-0_d.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/CEGUI/libCEGUIExpatParser_d.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/CEGUI/libCEGUIOpenGLRenderer-0_d.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/glew/libGLEW.2.0.0.dylib
  ${CMAKE_SOURCE_DIR}/../../deps/lib/Box2D/libBox2D.a)
