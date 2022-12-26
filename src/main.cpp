#include "Exceptions/Exception.h"
#include "Game.h"
#include "Utils.h"
#include <cstdlib>

int main()
{
    try {
        WizardGame::Game game;

        return game.run();
    } catch (WizardGame::Exception const& except) {
        WizardGame::error() << "Failed to run the game due to an error!\n\t" << except.what() << "\n";
        return EXIT_FAILURE;
    }
}
