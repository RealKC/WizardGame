#pragma once

#include "Level.h"

namespace WizardGame {

class TutorialLevel final : public Level {
public:
    TutorialLevel(uint32_t level_event);
    virtual ~TutorialLevel() {}

private:
    // ^Level
    virtual void run_frame_impl(uint32_t current_time) override;
    virtual void render_impl(SDL_Renderer*, TextRenderer&, SpriteManager&) override;
    virtual void dismiss_dialogue_if_any() override;
    virtual BackgroundId background_id() const override { return BackgroundId::Tutorial; };
    virtual void kill_player() override;
    virtual void restart_level() override;

    static constexpr int DIALOGUE_COUNT = 3;

    int m_tutorial_point;
};

}
