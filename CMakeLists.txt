cmake_minimum_required(VERSION 2.6)

project(taisto2)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
link_libraries(${SDL_LIBRARY} ${OPENGL_LIBRARY})
include_directories(${SDL_INCLUDE_DIR})

add_definitions("-Wall -O2")

set(sources main.cpp physics.cpp Server.cpp Game.cpp ClientSocket.cpp Server.cpp ClientInfo.cpp)

#MESSAGE( STATUS "flags : ${OPENGL_LIBRARY}" )

add_executable(taisto2 ${sources})
