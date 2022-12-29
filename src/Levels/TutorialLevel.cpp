#include "TutorialLevel.h"

#include "../Utils.h"
#include "Enemies/Attack.h"
#include "Enemies/Basic.h"

namespace WizardGame {

struct Dialog {
    PortraitId portrait_id;
    char const* name;
    char const* tip;
};

// NOTE: It'd be nice if those were read from a file.
static Dialog dialogs[] = {
    { PortraitId::Matei, "Matei", "Mircea este timpul să mergem după Adrian, nu-l putem lăsa să distruga Carpații!" },
    { PortraitId::Matei, "Matei", "Folosește tastele cu săgeți pentru a te mișca și tasta X pentru a trage bile de foc" },
    { PortraitId::Matei, "Matei (gândindu-se)", "Nu mă așteptam să ne pice de folos obsesia lui Mircea cu bilele de foc. Chiar a făcut bine că a memorizat vraja aceea, considerând că Adrian ne-a furat cărțile cu vrăji..." },
    { PortraitId::Matei, "Matei", "Hai" },
    { PortraitId::Matei, "Matei", "Ai grijă! Au apărut minionii lui Adrian! Arată-le focul tău!!!" },
    { PortraitId::Matei, "Matei", "Agghh!! Unul din minioni m-a nimerit! Nu mai există salvare pentru mine..." },
    { PortraitId::Mihai, "Mihai (panicat)", "Sensei!" },
    { PortraitId::Mircea, "Mircea (panicat)", "-! Sensei!" },
    { PortraitId::Matei, "Matei (pe moarte)", "Băieți... A venit sfârșitul pentru mine... Oricum eram prea bătrân.. Ha, ha, ha *cof cof cof*" },
    { PortraitId::Mihai, "Mihai", "Chiar nu există vreun mod prin care să te salvăm!?" },
    { PortraitId::Matei, "Matei (la un enunț depărtare de moarte)", "Nu... Dar e ok, toți ne întâlnim cu moarte mai devreme sau mai târziu... Mihai... nu te mai... învinovăți pentru ce a avut loc in '59..." },
    { PortraitId::Mircea, "Mircea", "Sensei..." },
    { PortraitId::Mircea, "Mircea (hotărât)", "Jur pe viața mea că te voi rărzbuna Sensei!!" },
    { PortraitId::Mihai, "Mihai", "Mircea. Îmi pare rău că nu te pot ajuta să-l răzbuni pe Sensei în persoană, voi ajuta cu eforturile de evacuare. Nu vreau să aiba și alți oameni experiențe similare cu ce s-a întâmplat nouă." },
    { PortraitId::Mihai, "Mihai (gândindu-se)", "Poate dacă nu-mi pierdeam mâna in '59..." },
    { PortraitId::Mircea, "Mircea", "Succes! Am încredere în tine." },
};

static constexpr std::size_t DIALOGUE_COUNT = std::size(dialogs);

TutorialLevel::TutorialLevel(uint32_t level_event)
    : Level(level_event,
        { (Level::PLAYING_AREA_RIGHT_LIMIT - Level::PLAYING_AREA_LEFT_LIMIT) / 2,
            (Level::PLAYING_AREA_BOTTOM_LIMIT - Level::PLAYING_AREA_TOP_LIMIT) / 2 })
    , m_tutorial_stage(Stage::Intro)
    , m_keys_pressed(0)
    , m_has_been_won(false)
    , m_dialog_index(0)
{
    set_title("Home");
}

void TutorialLevel::run_frame_impl(uint32_t current_time)
{
    switch (m_tutorial_stage) {
    case Stage::GoodLuck:
    case Stage::Shooting:
        tick_enemies(current_time);
        check_collisions();
    case Stage::Keyboard:
        handle_player_keypresses(current_time);
        update_bullet_positions();
    case Stage::Intro:
        break;
    }
}

void TutorialLevel::render_impl(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager& sprite_manager)
{
    if (m_tutorial_stage >= Stage::Keyboard) {
        render_bullets(renderer, sprite_manager);
        render_entities(renderer, sprite_manager);
    }

    auto& dialog = dialogs[m_dialog_index];

    render_dialog(renderer, text_renderer, sprite_manager, dialog.portrait_id, dialog.name, dialog.tip);
}

void TutorialLevel::dismiss_dialogue_if_any()
{
    if (!can_progress_tutorial()) {
        return;
    }

    if ((m_dialog_index + 1) == DIALOGUE_COUNT) {
        m_has_been_won = true;
    }

    if (m_dialog_index + 1 < DIALOGUE_COUNT) {
        m_dialog_index++;
    }

    switch (m_dialog_index) {
    case Stage::Keyboard:
        m_tutorial_stage = Stage::Keyboard;
        break;
    case Stage::Shooting: {
        using Enemies::Attack;

        m_player_bullets.clear();

        Attack attack { Attack::Type::Line, 1000, 0 };
        auto playing_area_middle = (Level::PLAYING_AREA_RIGHT_LIMIT - Level::PLAYING_AREA_LEFT_LIMIT) / 2;
        m_enemies.push_back(std::make_unique<Enemies::Basic>(
            Collider { playing_area_middle + 100, 50, 50, 50 },
            Vec2 { playing_area_middle + 100, 50 },
            std::vector { attack }));
        m_enemies.push_back(std::make_unique<Enemies::Basic>(
            Collider { playing_area_middle - 100, 50, 50, 50 },
            Vec2 { playing_area_middle - 100, 50 },
            std::vector { attack }));
        m_enemies.push_back(std::make_unique<Enemies::Basic>(
            Collider { playing_area_middle, 100, 50, 50 },
            Vec2 { playing_area_middle, 100 },
            std::vector { attack }));

        m_tutorial_stage = Stage::Shooting;
        break;
    }
    case Stage::GoodLuck:
        m_player_bullets.clear();
        m_enemy_bullets.clear();
        m_tutorial_stage = Stage::GoodLuck;
        break;
    }
}

void TutorialLevel::kill_player()
{
    // Player can't die in the tutorial
}

void TutorialLevel::restart_level()
{
    Level::restart_level();
    m_dialog_index = 0;
}

void TutorialLevel::scancode_hook(SDL_Scancode scancode)
{
    switch (scancode) {
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_X:
        m_keys_pressed++;
        break;
    }
}

bool TutorialLevel::has_been_won() const
{
    return m_has_been_won;
}

bool TutorialLevel::can_progress_tutorial() const
{
    if (m_tutorial_stage < Stage::Keyboard) {
        return true;
    }

    if (m_tutorial_stage == Stage::Keyboard && m_keys_pressed >= 5) {
        return true;
    }

    if (m_tutorial_stage == Stage::Shooting && m_enemies.empty()) {
        return true;
    }

    if (m_tutorial_stage == Stage::GoodLuck) {
        return true;
    }

    return false;
}

}
