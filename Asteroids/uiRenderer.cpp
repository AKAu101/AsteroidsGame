#include "uirenderer.h"
#include "raymath.h"
#include "globals.h"

UIRenderer::UIRenderer(GameStateManager& stateMgr, Score& score, Spaceship& ship, ObjectManager& objMgr, int& currentItemRef) :
    stateManager(stateMgr), gameScore(score), player(ship), objectManager(objMgr), currentItem(currentItemRef) {
}

void UIRenderer::DrawCurrentState() const {
    switch (stateManager.GetCurrentState()) {
    case MAIN_MENU:
        DrawMainMenu();
        break;
    case IN_GAME:
        DrawInGame();
        break;
    case GAME_OVER:
        DrawGameOver();
        break;
    case OPTIONS:
        DrawOptionsMenu();
        break;
    default:
        DrawMainMenu(); // Fallback
        break;
    }
}

void UIRenderer::DrawCheckeredBackground() const {
    ClearBackground(WHITE);

    // Vertikale Linien
    for (int x = 0; x < SCREEN_WIDTH; x += 40) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, LIGHTGRAY);
    }

    // Horizontale Linien
    for (int y = 0; y < SCREEN_HEIGHT; y += 40) {
        DrawLine(0, y, SCREEN_WIDTH, y, LIGHTGRAY);
    }
}

void UIRenderer::DrawMainMenu() const {
    // MS Paint-style Hintergrund
    ClearBackground(WHITE);
    DrawRectangle(10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, LIGHTGRAY);
    DrawRectangleLines(10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, BLACK);

    // Titel
    const char* title = "ASTEROIDS";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 150, 60, BLACK);

    // Untertitel
    const char* subtitle = "Classic Space Shooter";
    int subtitleWidth = MeasureText(subtitle, 20);
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, 220, 20, DARKGRAY);

    // Menü-Optionen
    const char* items[] = { "START GAME", "CONTROLS", "QUIT" };
    int menuStartY = 300;
    int menuItemHeight = 70;
    int menuItemWidth = 250;

    for (int i = 0; i < 3; i++) {
        int itemY = menuStartY + i * menuItemHeight;
        int itemX = SCREEN_WIDTH / 2 - menuItemWidth / 2;

        // Ausgewähltes Item hervorheben
        Color bgColor = (i == stateManager.GetMenuSelection()) ? RED : WHITE;
        Color textColor = (i == stateManager.GetMenuSelection()) ? WHITE : BLACK;
        Color borderColor = (i == stateManager.GetMenuSelection()) ? MAROON : BLACK;

        // Button zeichnen
        DrawRectangle(itemX, itemY, menuItemWidth, 50, bgColor);
        DrawRectangleLines(itemX, itemY, menuItemWidth, 50, borderColor);

        // Text zentriert zeichnen
        int textWidth = MeasureText(items[i], 24);
        DrawText(items[i], itemX + (menuItemWidth - textWidth) / 2, itemY + 13, 24, textColor);
    }

    // Steuerungshinweise
    const char* controls = "Use UP/DOWN arrows or W/S to navigate, ENTER to select";
    int controlsWidth = MeasureText(controls, 16);
    DrawText(controls, SCREEN_WIDTH / 2 - controlsWidth / 2, SCREEN_HEIGHT - 60, 16, DARKGRAY);

    // Version/Credits (optional)
    const char* version = "v1.0 - Classic Asteroids Clone";
    int versionWidth = MeasureText(version, 12);
    DrawText(version, SCREEN_WIDTH / 2 - versionWidth / 2, SCREEN_HEIGHT - 30, 12, GRAY);
}

void UIRenderer::DrawInGame() const {
    // Hintergrund zeichnen
    DrawCheckeredBackground();

    // Alle Spielobjekte zeichnen
    DrawGameObjects();

    // UI-Overlay zeichnen
    DrawGameUI();
}

void UIRenderer::DrawGameObjects() const {
    // Raumschiff zeichnen (mit allen Effekten wie Schild, falls aktiv)
    player.Draw();

    // Alle Projektile zeichnen
    for (const auto& projectile : objectManager.GetProjectiles()) {
        if (projectile.IsActive()) {
            projectile.Draw();
        }
    }

    // Alle Asteroiden zeichnen
    for (const auto& asteroid : objectManager.GetAsteroids()) {
        if (asteroid.IsActive()) {
            asteroid.Draw();
        }
    }

    // PowerUps zeichnen
    for (const auto& powerup : objectManager.GetPowerUps()) {
        if (powerup.IsActive()) {
            powerup.Draw();
        }
    }
}

void UIRenderer::DrawGameUI() const {
    // Haupt-UI Panel (links oben)
    int mainUIWidth = 250;
    int mainUIHeight = 140;
    DrawRectangle(5, 5, mainUIWidth, mainUIHeight, Color{ 0, 0, 0, 150 });
    DrawRectangleLines(5, 5, mainUIWidth, mainUIHeight, WHITE);

    // Basis-Informationen
    DrawText(TextFormat("SCORE: %d", gameScore.GetScore()), 15, 15, 20, WHITE);
    DrawText(TextFormat("LIVES: %d", player.GetLives()), 15, 40, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", gameScore.GetLevel()), 15, 65, 20, WHITE);
    DrawText(TextFormat("HIGH: %d", gameScore.GetHighScore()), 15, 90, 20, YELLOW);

    // Item-Slot-Anzeige (rechts oben)
    DrawItemSlot();

    // Schild-Status anzeigen
    if (player.IsShieldActive()) {
        float timeRemaining = player.GetShieldTimeRemaining();
        DrawText(TextFormat("SHIELD: %.1fs", timeRemaining), 15, 115, 16, BLUE);
    }

    // Unverwundbarkeits-Status anzeigen
    if (player.IsInvulnerable()) {
        int yOffset = player.IsShieldActive() ? 135 : 115; // Position anpassen je nach Schild-Status
        DrawText("INVULNERABLE", 15, yOffset, 16, YELLOW);
    }

    // Steuerungshinweise (rechts unten)
    DrawText("ESC: Menu", SCREEN_WIDTH - 100, 10, 12, LIGHTGRAY);
    DrawText("SHIFT: Use Item", SCREEN_WIDTH - 120, SCREEN_HEIGHT - 25, 12, LIGHTGRAY);
}

void UIRenderer::DrawItemSlot() const {
    // Item-Slot Position (rechts oben)
    int slotX = SCREEN_WIDTH - 120;
    int slotY = 15;
    int slotSize = 80;

    // Slot-Hintergrund
    DrawRectangle(slotX, slotY, slotSize, slotSize, Color{ 40, 40, 40, 200 });
    DrawRectangleLines(slotX, slotY, slotSize, slotSize, WHITE);

    // Slot-Titel
    DrawText("ITEM SLOT", slotX - 20, slotY - 15, 12, WHITE);

    // Item-Inhalt anzeigen
    if (currentItem > 0) {
        // Item-Eigenschaften basierend auf Typ
        const char* itemNames[] = { "", "RAPID", "SHIELD", "LIFE" };
        Color itemColors[] = { WHITE, RED, BLUE, GREEN };
        const char* itemSymbols[] = { "", "⚡", "🛡", "❤" };

        if (currentItem >= 1 && currentItem <= 3) {
            // Item-Hintergrund
            Color bgColor = itemColors[currentItem];
            bgColor.a = 150; // Mehr Sichtbarkeit
            DrawRectangle(slotX + 5, slotY + 5, slotSize - 10, slotSize - 10, bgColor);

            // Item-Symbol/Text (einfacher Text da Symbole problematisch sein können)
            const char* symbols[] = { "", "R", "S", "L" }; // Buchstaben statt Symbole
            int symbolWidth = MeasureText(symbols[currentItem], 32);
            DrawText(symbols[currentItem],
                slotX + (slotSize - symbolWidth) / 2,
                slotY + 15, 32, WHITE);

            // Item-Name
            int nameWidth = MeasureText(itemNames[currentItem], 12);
            DrawText(itemNames[currentItem],
                slotX + (slotSize - nameWidth) / 2,
                slotY + 50, 12, WHITE);

            // Aktivierungs-Hinweis
            DrawText("SHIFT", slotX + (slotSize - MeasureText("SHIFT", 10)) / 2,
                slotY + 65, 10, YELLOW);

            // Pulsierender Rahmen für aktives Item
            static float pulseTimer = 0;
            pulseTimer += GetFrameTime();
            float pulse = 0.7f + 0.3f * sinf(pulseTimer * 5.0f);
            Color pulseColor = itemColors[currentItem];
            pulseColor.a = (unsigned char)(255 * pulse);
            DrawRectangleLines(slotX + 2, slotY + 2, slotSize - 4, slotSize - 4, pulseColor);

            // Extra Debug-Text
            DrawText(TextFormat("ID:%d", currentItem), slotX + 5, slotY + 75, 8, WHITE);
        }
    }
    else {
        // Leerer Slot
        DrawText("EMPTY", slotX + 20, slotY + 30, 12, GRAY);
        DrawText("Collect", slotX + 15, slotY + 45, 10, GRAY);
        DrawText("PowerUps", slotX + 12, slotY + 57, 10, GRAY);
    }

    // Slot-Rahmen verstärken
    DrawRectangleLines(slotX - 1, slotY - 1, slotSize + 2, slotSize + 2, WHITE);
}

void UIRenderer::DrawGameOver() const {
    // Dunkler Hintergrund
    ClearBackground(BLACK);

    // Haupttext
    const char* gameOverText = "GAME OVER";
    int gameOverWidth = MeasureText(gameOverText, 60);
    DrawText(gameOverText, SCREEN_WIDTH / 2 - gameOverWidth / 2, SCREEN_HEIGHT / 2 - 100, 60, RED);

    // Score-Informationen
    const char* finalScore = TextFormat("FINAL SCORE: %d", gameScore.GetScore());
    int scoreWidth = MeasureText(finalScore, 30);
    DrawText(finalScore, SCREEN_WIDTH / 2 - scoreWidth / 2, SCREEN_HEIGHT / 2 - 20, 30, WHITE);

    const char* highScore = TextFormat("HIGH SCORE: %d", gameScore.GetHighScore());
    int highScoreWidth = MeasureText(highScore, 24);
    DrawText(highScore, SCREEN_WIDTH / 2 - highScoreWidth / 2, SCREEN_HEIGHT / 2 + 20, 24, YELLOW);

    // Neuer High Score?
    if (gameScore.GetScore() == gameScore.GetHighScore() && gameScore.GetScore() > 0) {
        const char* newRecord = "NEW HIGH SCORE!";
        int recordWidth = MeasureText(newRecord, 20);
        DrawText(newRecord, SCREEN_WIDTH / 2 - recordWidth / 2, SCREEN_HEIGHT / 2 + 60, 20, GREEN);
    }

    // Anweisungen
    const char* instruction1 = "Press ENTER to return to menu";
    int inst1Width = MeasureText(instruction1, 20);
    DrawText(instruction1, SCREEN_WIDTH / 2 - inst1Width / 2, SCREEN_HEIGHT / 2 + 100, 20, LIGHTGRAY);

    const char* instruction2 = "Press R for quick restart";
    int inst2Width = MeasureText(instruction2, 16);
    DrawText(instruction2, SCREEN_WIDTH / 2 - inst2Width / 2, SCREEN_HEIGHT / 2 + 130, 16, GRAY);

    const char* instruction3 = "Press ESC to quit";
    int inst3Width = MeasureText(instruction3, 16);
    DrawText(instruction3, SCREEN_WIDTH / 2 - inst3Width / 2, SCREEN_HEIGHT / 2 + 150, 16, GRAY);
}

void UIRenderer::DrawOptionsMenu() const {
    // Controls-Menü Hintergrund
    ClearBackground(DARKGRAY);
    DrawRectangle(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60, LIGHTGRAY);
    DrawRectangleLines(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60, BLACK);

    // Titel
    const char* title = "CONTROLS";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 60, 40, BLACK);

    // Untertitel
    const char* subtitle = "Master the art of space flight";
    int subtitleWidth = MeasureText(subtitle, 16);
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, 110, 16, DARKGRAY);

    // Controls-Kategorien
    int startY = 150;
    int lineHeight = 25;
    int currentY = startY;

    // Movement Section
    DrawText("MOVEMENT:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("W / UP ARROW", 80, currentY, 16, BLACK);
    DrawText("- Thrust forward", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("A / LEFT ARROW", 80, currentY, 16, BLACK);
    DrawText("- Rotate left", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("D / RIGHT ARROW", 80, currentY, 16, BLACK);
    DrawText("- Rotate right", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight + 15;

    // Combat Section
    DrawText("COMBAT:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("SPACEBAR", 80, currentY, 16, BLACK);
    DrawText("- Fire projectiles", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("SHIFT", 80, currentY, 16, BLACK);
    DrawText("- Use collected item", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight + 15;

    // Items Section
    DrawText("POWER-UPS:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("RAPID FIRE (Red)", 80, currentY, 16, RED);
    DrawText("- Faster shooting for 50 shots", 260, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("SHIELD (Blue)", 80, currentY, 16, BLUE);
    DrawText("- Absorbs one asteroid hit", 260, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("EXTRA LIFE (Green)", 80, currentY, 16, GREEN);
    DrawText("- Instantly adds one life", 260, currentY, 16, DARKGRAY);
    currentY += lineHeight + 15;

    // Navigation Section
    DrawText("NAVIGATION:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("ESC", 80, currentY, 16, BLACK);
    DrawText("- Return to main menu / Quit game", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("R", 80, currentY, 16, BLACK);
    DrawText("- Quick restart (Game Over screen)", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight + 20;

    // Tips Section
    DrawText("TIPS:", 60, currentY, 20, MAROON);
    currentY += 25;

    DrawText("• Destroy large asteroids to create smaller ones", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• Collect power-ups by flying over them", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• You are invulnerable for 3 seconds after respawning", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• Power-ups spawn every 15 seconds or from large asteroids", 80, currentY, 14, DARKBLUE);

    // Zurück-Anweisung
    const char* backText = "Press ENTER or ESC to return to main menu";
    int backWidth = MeasureText(backText, 18);
    DrawText(backText, SCREEN_WIDTH / 2 - backWidth / 2, SCREEN_HEIGHT - 80, 18, BLACK);

    // Dekorative Elemente
    DrawRectangleLines(50, 140, SCREEN_WIDTH - 100, 2, BLACK);
    DrawRectangleLines(50, SCREEN_HEIGHT - 100, SCREEN_WIDTH - 100, 2, BLACK);
}