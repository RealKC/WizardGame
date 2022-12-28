#pragma once

#include "Menu.h"

namespace WizardGame {
namespace UI {

class GameOverMenu : public Menu {
public:
    enum ActivationResult {
        Restart,
        QuitToMenu,
        QuitToDesktop,
    };

    virtual int activate_current_selection() override;
    virtual void deactivate_current_selection() override;
    virtual void render(SDL_Renderer* renderer, TextRenderer& text_renderer) const override;

protected:
    virtual int max_menu_item() const override { return ActivationResult::QuitToDesktop; }

private:
};

}
}
