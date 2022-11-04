cmake_minimum_required(VERSION 3.22)
project(WizardGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(WizardGame src/main.cpp src/Game.cpp src/Game.h src/Utils.h src/Utils.cpp src/Entity.cpp src/Entity.h src/Player.cpp src/Player.h src/Vec2.h src/KeyboardState.cpp src/KeyboardState.h)

find_package(SDL2 REQUIRED)
message(STATUS ${SDL2_INCLUDE_DIRS})
target_link_libraries(WizardGame SDL2::SDL2)
