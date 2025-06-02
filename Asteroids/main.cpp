#include "game.h"
#include "gamestate.h"
#include "game.h"
#include <iostream>
#include <exception>

int main() {
    try {
        Game game;
        game.Initialize();

        while (!WindowShouldClose() && game.IsGameRunning()) {
            try {
                game.Update();
                game.Draw();
            }
            catch (const std::exception& e) {
                std::cerr << "Error in game loop: " << e.what() << std::endl;
                // Continue the loop - try to recover
                continue;
            }
            catch (...) {
                std::cerr << "Unknown error in game loop!" << std::endl;
                // Continue the loop - try to recover
                continue;
            }
        }

        CloseWindow();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Game error: " << e.what() << std::endl;
        
        // Try to close window safely if it was opened
        if (IsWindowReady()) {
            CloseWindow();
        }
        
        return -1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred in game!" << std::endl;
        
        // Try to close window safely if it was opened
        if (IsWindowReady()) {
            CloseWindow();
        }
        
        return -2;
    }
}