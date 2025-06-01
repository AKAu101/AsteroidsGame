#include "game.h"
#include "gamestate.h"
#include "game.h"

int main() {
    Game game;
    game.Initialize();

    // Hauptschleife - hier ist der wichtige Teil!
    while (!WindowShouldClose() && game.IsGameRunning()) {
        game.Update();
        game.Draw();
    }

    CloseWindow();
    return 0;
}