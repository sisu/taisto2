cmake_minimum_required(VERSION 2.6)

project(taisto2)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_LIBRARY} "SDL")
include_directories(${SDL_INCLUDE_DIR})

#add_definitions("-Wall -g")
#add_definitions("-Wall -O2")
add_definitions("-Wall -g -Wno-unused-result")
#add_definitions("-Wall -O2")
#add_definitions("-Wall -g -Wno-unused-result")
#add_definitions("-Wall -Wno-unused-result -Os -fomit-frame-pointer -ffast-math")

set(sources main.cpp physics.cpp Server.cpp Game.cpp ClientSocket.cpp
        ClientInfo.cpp Connection.cpp DataWriter.cpp Bullet.cpp texgen.cpp
		Bot.cpp salama.cpp explosion.cpp LCD.cpp Menu.cpp input.cpp Model.cpp
		music.cpp HUD.cpp)

#MESSAGE( STATUS "flags : ${OPENGL_LIBRARY}" )

add_executable(taisto2 ${sources})
add_executable(mtest mtest.cpp music.cpp)
