#include "uirenderer.h"
#include "raymath.h"
#include "globals.h"

UIRenderer::UIRenderer(GameStateManager& stateMgr, Score& score, Spaceship& ship, ObjectManager& objMgr) :
    stateManager(stateMgr), gameScore(score), player(ship), objectManager(objMgr) {
}

void UIRenderer::DrawCurrentState() const {
    switch (stateManager.GetCurrentState()) {
    case MAIN_MENU: DrawMainMenu(); break;
    case IN_GAME: DrawInGame(); break;
    case GAME_OVER: DrawGameOver(); break;
    case OPTIONS: /* Draw options */ break;
    }
}

void UIRenderer::DrawCheckeredBackground() const {
    ClearBackground(WHITE);
    for (int x = 0; x < SCREEN_WIDTH; x += 40) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, LIGHTGRAY);
    }
    for (int y = 0; y < SCREEN_HEIGHT; y += 40) {
        DrawLine(0, y, SCREEN_WIDTH, y, LIGHTGRAY);
    }
}

void UIRenderer::DrawMainMenu() const {
    // MS Paint-style menu
    ClearBackground(WHITE);
    DrawRectangle(10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, LIGHTGRAY);

    // Title
    const char* title = "ASTEROIDS";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 200, 40, BLACK);

    // Menu items
    const char* items[] = { "START GAME", "OPTIONS", "QUIT" };
    for (int i = 0; i < 3; i++) {
        Color color = (i == stateManager.GetMenuSelection()) ? RED : WHITE;
        DrawRectangle(SCREEN_WIDTH / 2 - 100, 350 + i * 60, 200, 40, color);
        DrawText(items[i], SCREEN_WIDTH / 2 - MeasureText(items[i], 20) / 2, 360 + i * 60, 20, BLACK);
    }
}

void UIRenderer::DrawInGame() const {
    DrawCheckeredBackground();

    // Zeichne alle Spielobjekte
    DrawGameObjects();

    // Zeichne UI (Score, Lives, Level)
    DrawRectangle(5, 5, 200, 100, Color{ 0, 0, 0, 150 });
    DrawText(TextFormat("SCORE: %d", gameScore.GetScore()), 10, 10, 20, WHITE);
    DrawText(TextFormat("LIVES: %d", player.GetLives()), 10, 40, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", gameScore.GetLevel()), 10, 70, 20, WHITE);
}

void UIRenderer::DrawGameObjects() const {
    // Zeichne das Raumschiff
    player.Draw();

    // Zeichne alle Projektile
    for (const auto& projectile : objectManager.GetProjectiles()) {
        projectile.Draw();
    }

    // Zeichne alle Asteroiden
    for (const auto& asteroid : objectManager.GetAsteroids()) {
        asteroid.Draw();
    }

    // PowerUps - falls GetPowerUps() in ObjectManager implementiert ist
    // Entkommentiere diese Zeilen wenn GetPowerUps() verfügbar ist:
    /*
    for (const auto& powerup : objectManager.GetPowerUps()) {
        powerup.Draw();
    }
    */
}

void UIRenderer::DrawGameOver() const {
    ClearBackground(BLACK);
    DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 30, RED);
    DrawText(TextFormat("SCORE: %d", gameScore.GetScore()),
        SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, WHITE);
    DrawText(TextFormat("HIGH SCORE: %d", gameScore.GetHighScore()),
        SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 30, 20, WHITE);
    DrawText("Press ENTER to return to menu",
        SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 80, 15, WHITE);
}