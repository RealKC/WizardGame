#pragma once

#include "AbstractLevelMenu.h"

namespace WizardGame {
namespace UI {

class LevelCompletedMenu : public AbstractLevelMenu {
public:
    enum ActivationResult {
        NextLevel = 8,
    };

    LevelCompletedMenu(bool has_next_level, bool got_new_high_score)
        : AbstractLevelMenu(custom_activation_results(has_next_level), title(has_next_level), items(has_next_level))
        , m_selection_offset(has_next_level ? 1 : 2)
        , m_got_new_high_score(got_new_high_score)
    {
    }

protected:
    virtual int selection_offset() const override { return m_selection_offset; }
    virtual bool has_subtitle() const override { return m_got_new_high_score; }
    virtual std::string subtitle() const override { return "!!! New High Score !!!"; }

private:
    static std::vector<int> custom_activation_results(bool has_next_level);
    static std::string title(bool has_next_level);
    static std::vector<char const*> items(bool has_next_level);

    int m_selection_offset;
    bool m_got_new_high_score;
};

}
}
