#pragma once

#include "AbstractLevelMenu.h"

namespace WizardGame {
namespace UI {

class LevelCompletedMenu : public AbstractLevelMenu {
public:
    enum ActivationResult {
        NextLevel = 8,
    };

    LevelCompletedMenu(bool has_next_level)
        : AbstractLevelMenu(custom_activation_results(has_next_level), title(has_next_level), items(has_next_level))
        , m_selection_offset(has_next_level ? 1 : 2)
    {
    }

protected:
    virtual int selection_offset() const override { return m_selection_offset; }

private:
    static std::vector<int> custom_activation_results(bool has_next_level);
    static std::string title(bool has_next_level);
    static std::vector<char const*> items(bool has_next_level);

    int m_selection_offset;
};

}
}
