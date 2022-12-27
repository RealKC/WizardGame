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
        src/Levels/Entity.cpp src/Levels/Entity.h
        src/Levels/Player.cpp src/Levels/Player.h
        src/KeyboardState.cpp src/KeyboardState.h
        src/Levels/Collider.cpp src/Levels/Collider.h
        src/Levels/Bullet.cpp src/Levels/Bullet.h
        src/Levels/Enemy.cpp src/Levels/Enemy.h
        src/Levels/Enemies/Basic.cpp src/Levels/Enemies/Basic.h
        src/Levels/Enemies/Adrian.cpp src/Levels/Enemies/Adrian.h
        src/Levels/Enemies/Attack.h
        src/Levels/Level.cpp src/Levels/Level.h
        src/Levels/FileLevel.cpp src/Levels/FileLevel.h
        src/SpriteManager.cpp src/SpriteManager.h
        src/TextRenderer.cpp src/TextRenderer.h
        src/UI/Menu.cpp src/UI/Menu.h
        src/UI/MainMenu.cpp src/UI/MainMenu.h
        src/UI/PauseMenu.cpp src/UI/PauseMenu.h
        src/Exceptions/Exception.cpp src/Exceptions/Exception.h
        src/Exceptions/InitError.h
        src/Exceptions/SDLObjectError.h
        src/Exceptions/TTFObjectError.cpp src/Exceptions/TTFObjectError.h
        src/UserEvents.h
        )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)


target_link_libraries(WizardGame SDL2::Core SDL2::Image SDL2::TTF)
