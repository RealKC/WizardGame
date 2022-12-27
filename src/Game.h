#pragma once

#include "KeyboardState.h"
#include "Levels/Level.h"
#include "SpriteManager.h"
#include "TextRenderer.h"
#include "UI/MainMenu.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace WizardGame {

class Game {
public:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 960;

    Game();
    ~Game();

    int run();

private:
    bool is_displaying_menu() const { return m_level == nullptr; }

    void event_loop();
    void render(uint32_t start_ticks);

    UI::MainMenu m_menu;

    Uint32 const m_level_event;

    // Game state
    std::unique_ptr<Level> m_level;

    bool m_quit;

    // Render state
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SpriteManager m_sprite_manager;
    TextRenderer m_text_renderer;
};

}
