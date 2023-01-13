#include "Exceptions/Exception.h"
#include "Game.h"
#include "Utils.h"
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

int main(int, char**)
{
    // Ensure the "saves/" directory exists so opening files like "saves/level" works...
    fs::create_directory("saves");

    try {
        WizardGame::Game game;

        return game.run();
    } catch (WizardGame::Exception const& except) {
        WizardGame::error() << "Failed to run the game due to an error!\n\t" << except.what() << "\n";
        return EXIT_FAILURE;
    }
}
