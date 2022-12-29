#pragma once

#include "Level.h"

namespace WizardGame {

class TutorialLevel final : public Level {
public:
    explicit TutorialLevel(uint32_t level_event);
    virtual ~TutorialLevel() { }

private:
    // ^Level
    virtual void run_frame_impl(uint32_t current_time) override;
    virtual void render_impl(SDL_Renderer*, TextRenderer&, SpriteManager&) override;
    virtual void dismiss_dialogue_if_any() override;
    virtual BackgroundId background_id() const override { return BackgroundId::Tutorial; };
    virtual void kill_player() override;
    virtual void restart_level() override;
    virtual void scancode_hook(SDL_Scancode) override;
    virtual int next_level() const override { return 1; }
    virtual bool has_been_won() const override;

    bool can_progress_tutorial() const;

    enum Stage {
        Intro = 0,
        Keyboard = 3,
        Shooting,
        GoodLuck,
    };

    Stage m_tutorial_stage;
    int m_keys_pressed;
    bool m_has_been_won;
    std::size_t m_dialog_index;
};

}
