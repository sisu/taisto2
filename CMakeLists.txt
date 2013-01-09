cmake_minimum_required(VERSION 2.6)

project(taisto2)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_LIBRARY} "SDL")
include_directories(${SDL_INCLUDE_DIR})

#add_definitions("-Wall -g")
#add_definitions("-Wall -O2")
#add_definitions("-Wall -g -Wno-unused-result ")
#add_definitions("-Wall -O2 -g -fno-inline")
#add_definitions("-Wall -g -Wno-unused-result")
add_definitions("-Wall -Wno-unused-result")
set(sopt "-Os -fomit-frame-pointer -ffast-math -fno-exceptions")
set(dopt "-Wall -O2 -g -fno-inline")

set(snd music.cpp padsynth.cpp mplay.cpp)
set(sources main.cpp physics.cpp Server.cpp Game.cpp ClientSocket.cpp
        ClientInfo.cpp Connection.cpp DataWriter.cpp Bullet.cpp texgen.cpp
		Bot.cpp salama.cpp Particle.cpp LCD.cpp Menu.cpp input.cpp Model.cpp
		music.cpp HUD.cpp padsynth.cpp mplay.cpp)

#MESSAGE( STATUS "flags : ${OPENGL_LIBRARY}" )

add_executable(taisto2 ${sources})
set_target_properties(taisto2 PROPERTIES COMPILE_FLAGS ${dopt})
add_executable(mtest mplay.cpp)
set_target_properties(mtest PROPERTIES COMPILE_FLAGS "-DMTEST ${dopt}")
#add_executable(soundtest soundtest.cpp ${snd})
