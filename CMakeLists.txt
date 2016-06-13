cmake_minimum_required(VERSION 2.8.7)
project("Ganger Engine")

# Set the library type to Shared
SET (LIB_TYPE SHARED)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++14")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
    add_definitions(-DEFGE_USE_STD_RANDOMENGINE)
endif()

set(SOURCES
  src/AudioEngine.cpp
  src/Camera2D.cpp
  src/DebugRenderer.cpp
  src/GangerEngine.cpp
  src/GangerErrors.cpp
  src/GLSLProgram.cpp
  src/GUI.cpp
  src/IOManager.cpp
  src/ImageLoader.cpp
  src/IMainGame.cpp
  src/InputManager.cpp
  src/ParticleBatch2D.cpp
  src/ParticleEngine2D.cpp
  src/PicoPNG.cpp
  src/ResourceManager.cpp
  src/ScreenList.cpp
  src/Sprite.cpp
  src/SpriteBatch.cpp
  src/SpriteFont.cpp
  src/TextureCache.cpp
  src/Timing.cpp
  src/Window.cpp)

#Bring the headers into the project
include_directories(${CMAKE_SOURCE_DIR}/deps/include ${CMAKE_SOURCE_DIR}/include)

#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp" ${SDL_HEADERS})

add_library(GangerEngine ${SOURCES})
