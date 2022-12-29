#pragma once

#include "AbstractLevelMenu.h"

namespace WizardGame {
namespace UI {

class GameOverMenu final : public AbstractLevelMenu {
public:
    enum ActivationResult {
        Restart = 1,
    };

    GameOverMenu()
        : AbstractLevelMenu({ ActivationResult::Restart }, "-- Game Over --", { "Restart", "Quit to menu", "Quit to desktop" })
    {
    }

protected:
    virtual int selection_offset() const override { return 1; }
};

}
}
