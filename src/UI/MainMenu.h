#pragma once

#include "Menu.h"

namespace WizardGame {
namespace UI {

class MainMenu : public Menu {
public:
    MainMenu();

    enum ActivationResult {
        None,
        Quit,
        Tutorial,
        Level1,
        Level2,
        Level3,
    };

    virtual int activate_current_selection();
    virtual void deactivate_current_selection();

    virtual void render(SDL_Renderer* renderer, TextRenderer& text_renderer) const;

protected:
    virtual int max_menu_item() const;

private:
    void render_main_menu(SDL_Renderer* renderer, TextRenderer& text_renderer) const;
    void render_level_selection(SDL_Renderer* renderer, TextRenderer& text_renderer) const;

    bool m_is_in_level_selection;
};

} // WizardGame
} // UI
