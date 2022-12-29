#pragma once

#include "AbstractLevelMenu.h"

namespace WizardGame {
namespace UI {

class PauseMenu final : public AbstractLevelMenu {
public:
    enum ActivationResult {
        Continue = 5,
        Restart = 6,
    };

    PauseMenu()
        : AbstractLevelMenu({ ActivationResult::Continue, ActivationResult::Restart }, "-- Paused --", { "Continue", "Restart", "Quit to menu", "Quit to desktop" })
    {
    }

    virtual bool can_be_toggled() const override { return true; }
};

}
}
