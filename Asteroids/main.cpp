#include "game.h"
#include "gamestate.h"
#include "game.h"

int main() {
    Game game;
    game.Initialize();

    while (!WindowShouldClose() && game.IsGameRunning()) {
        game.Update();
        game.Draw();
    }

    CloseWindow();
    return 0;
}