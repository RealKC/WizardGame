#include "Menu.h"

namespace WizardGame {
namespace UI {

Menu::Menu()
    : m_selected_menu_item(0)
{
}

void Menu::select_next_item()
{
    if (m_selected_menu_item < max_menu_item()) {
        ++m_selected_menu_item;
    }
}

void Menu::select_previous_item()
{
    if (m_selected_menu_item > 0) {
        --m_selected_menu_item;
    }
}

}
}