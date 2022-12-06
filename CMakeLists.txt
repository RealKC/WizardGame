cmake_minimum_required(VERSION 3.22)
project(WizardGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(WizardGame
        src/main.cpp
        src/GenericTypes.h
        src/Size.h
        src/Vec2.h src/Vec2.cpp
        src/Game.cpp src/Game.h
        src/Utils.cpp src/Utils.h
        src/Entity.cpp src/Entity.h
        src/Player.cpp src/Player.h
        src/KeyboardState.cpp src/KeyboardState.h
        src/Collider.cpp src/Collider.h
        src/Bullet.cpp src/Bullet.h
        src/Enemy.cpp src/Enemy.h
        src/Enemies/Basic.cpp src/Enemies/Basic.h
        src/Enemies/Adrian.cpp src/Enemies/Adrian.h
        src/Enemies/Attack.h
        src/LevelManager.h src/LevelManager.cpp)

find_package(SDL2 REQUIRED)
message(STATUS ${SDL2_INCLUDE_DIRS})
target_link_libraries(WizardGame SDL2::SDL2)
