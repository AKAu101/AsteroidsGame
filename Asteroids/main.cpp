#include "game.h"
#include "gamestate.h"

int main() {
    // Initialize game state manager
    InitializeGameState();

    // Create game instance
    Game asteroids;

    // Initialize the game
    asteroids.Initialize();

    // Main game loop
    while (!WindowShouldClose() && !asteroids.ShouldClose()) {
        asteroids.Update();
        asteroids.Draw();
    }

    // Cleanup
    CloseWindow();
    return 0;
}