#pragma once

#include "Menu.h"
#include <vector>

namespace WizardGame {
namespace UI {

class AbstractLevelMenu : public Menu {
public:
    virtual ~AbstractLevelMenu() { }

    enum ActivationResult {
        QuitToMenu = 2,
        QuitToDesktop = 3,
    };

    virtual bool can_be_toggled() const { return false; }

    // ^Menu
    virtual int activate_current_selection() final;
    virtual void deactivate_current_selection() final;
    virtual void render(SDL_Renderer* renderer, TextRenderer& text_renderer) const final;

protected:
    AbstractLevelMenu(std::vector<int> custom_activation_results, std::string title, std::vector<char const*> items);

    virtual int selection_offset() const { return 0; }

    // ^Menu
    virtual int max_menu_item() const final { return ActivationResult::QuitToDesktop - selection_offset(); }

private:
    std::vector<int> m_activation_results;
    std::string m_title;
    std::vector<char const*> m_items;
};

}
}
