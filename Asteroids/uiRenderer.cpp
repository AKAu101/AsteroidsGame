#include "uirenderer.h"
#include "raymath.h"
#include "globals.h"
#include "highscoreManager.h"

/**
 * Constructor for UIRenderer class
 * Initializes references to game systems and state managers
 * Sets up dependencies for rendering different UI screens
 * @param stateMgr Reference to game state manager
 * @param score Reference to game score system
 * @param ship Reference to player spaceship
 * @param objMgr Reference to object manager
 * @param currentItemRef Reference to current power-up item
 */
UIRenderer::UIRenderer(GameStateManager& stateMgr, Score& score, Spaceship& ship, ObjectManager& objMgr, int& currentItemRef) :
    stateManager(stateMgr), gameScore(score), player(ship), objectManager(objMgr), currentItem(currentItemRef) {
}

/**
 * Main rendering dispatcher method
 * Routes rendering to appropriate state-specific methods
 * Provides fallback to main menu for unknown states
 */
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
    case HIGHSCORE_ENTRY:
        DrawNameEntryScreen();
        break;
    case HIGHSCORE_DISPLAY:
        DrawHighscoreScreen();
        break;
    case CREDITS:
        DrawCreditsScreen();
        break;
    default:
        DrawMainMenu(); // Fallback
        break;
    }
}

/**
 * Draws a checkered grid background pattern
 * Creates a white background with light gray grid lines
 * Used as background for in-game state
 */
void UIRenderer::DrawCheckeredBackground() const {
    ClearBackground(WHITE);

    // Vertical lines
    for (int x = 0; x < SCREEN_WIDTH; x += 40) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, LIGHTGRAY);
    }

    // Horizontal lines
    for (int y = 0; y < SCREEN_HEIGHT; y += 40) {
        DrawLine(0, y, SCREEN_WIDTH, y, LIGHTGRAY);
    }
}

/**
 * Renders the main menu with MS Paint-style aesthetic
 * Features animated title, colored menu buttons, decorative elements
 * Includes flying spaceships, asteroids, and various visual effects
 * Supports 5 menu options with visual feedback for selection
 */
void UIRenderer::DrawMainMenu() const {
    // Animation time for various effects
    static float animTime = 0;
    animTime += GetFrameTime();

    // MS Paint-style background with gradient
    ClearBackground(Color{ 220, 220, 220, 255 }); // Light gray

    // Decorative frames (like MS Paint windows)
    DrawRectangle(5, 5, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10, Color{ 192, 192, 192, 255 });
    DrawRectangleLines(5, 5, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10, BLACK);
    DrawRectangleLines(6, 6, SCREEN_WIDTH - 12, SCREEN_HEIGHT - 12, WHITE);
    DrawRectangleLines(7, 7, SCREEN_WIDTH - 14, SCREEN_HEIGHT - 14, BLACK);

    // Title area with MS Paint-style
    DrawRectangle(15, 15, SCREEN_WIDTH - 30, 120, Color{ 180, 180, 255, 255 }); // Bluish title area
    DrawRectangleLines(15, 15, SCREEN_WIDTH - 30, 120, BLACK);
    DrawRectangleLines(14, 14, SCREEN_WIDTH - 28, 122, WHITE);

    // Animated main title
    const char* title = "ASTEROIDS";
    int titleSize = 50;
    float titlePulse = 1.0f + 0.1f * sinf(animTime * 2.0f); // Light pulsing
    int titleWidth = MeasureText(title, (int)(titleSize * titlePulse));

    // Title shadow for 3D effect
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2 + 3, 50 + 3, (int)(titleSize * titlePulse), Color{ 100, 100, 100, 255 });
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 50, (int)(titleSize * titlePulse), BLACK);

    // Subtitle with animation
    const char* subtitle = "Classic Space Shooter - MS Paint Edition";
    int subtitleWidth = MeasureText(subtitle, 18);
    float subtitleFloat = sinf(animTime * 1.5f) * 3.0f; // Gentle floating
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, (int)(95 + subtitleFloat), 18, Color{ 80, 80, 80, 255 });

    // Creator credit
    const char* creator = "Created by Adrian Kranyak";
    int creatorWidth = MeasureText(creator, 14);
    DrawText(creator, SCREEN_WIDTH / 2 - creatorWidth / 2, 115, 14, Color{ 120, 60, 160, 255 }); // Purple color

    // Menu area
    DrawRectangle(50, 160, SCREEN_WIDTH - 100, 300, Color{ 240, 240, 240, 255 });
    DrawRectangleLines(50, 160, SCREEN_WIDTH - 100, 300, BLACK);
    DrawRectangleLines(49, 159, SCREEN_WIDTH - 98, 302, WHITE);

    // Menu options with animated buttons (now 5 options)
    const char* items[] = { "START GAME", "CONTROLS", "HIGHSCORES", "CREDITS", "QUIT" };
    Color itemColors[] = {
        Color{100, 255, 100, 255},   // Green for Start Game
        Color{100, 150, 255, 255},   // Blue for Controls  
        Color{255, 215, 0, 255},     // Gold for Highscores
        Color{255, 100, 255, 255},   // Magenta for Credits
        Color{255, 100, 100, 255}    // Red for Quit
    };
    int menuStartY = 170;
    int menuItemHeight = 55;
    int menuItemWidth = 280;

    for (int i = 0; i < 5; i++) {  // Changed to 5 options
        int itemY = menuStartY + i * menuItemHeight;
        int itemX = SCREEN_WIDTH / 2 - menuItemWidth / 2;

        // Button animation on selection
        bool isSelected = (i == stateManager.GetMenuSelection());
        float buttonPulse = isSelected ? 1.0f + 0.15f * sinf(animTime * 8.0f) : 1.0f;
        int buttonOffset = isSelected ? (int)(sinf(animTime * 6.0f) * 2.0f) : 0;

        // 3D Button effect (MS Paint style)
        Color bgColor = isSelected ? itemColors[i] : Color{ 220, 220, 220, 255 };
        Color shadowColor = Color{ 100, 100, 100, 255 };
        Color highlightColor = WHITE;

        // Button shadow
        DrawRectangle(itemX + 3, itemY + 3, menuItemWidth, 50, shadowColor);

        // Main button
        DrawRectangle(itemX + buttonOffset, itemY + buttonOffset, menuItemWidth, 50, bgColor);

        // Button highlights (3D effect)
        DrawRectangleLines(itemX + buttonOffset, itemY + buttonOffset, menuItemWidth, 50, BLACK);
        DrawLine(itemX + buttonOffset, itemY + buttonOffset, itemX + buttonOffset + menuItemWidth - 1, itemY + buttonOffset, highlightColor);
        DrawLine(itemX + buttonOffset, itemY + buttonOffset, itemX + buttonOffset, itemY + buttonOffset + 49, highlightColor);
        DrawLine(itemX + buttonOffset + 1, itemY + buttonOffset + 48, itemX + buttonOffset + menuItemWidth - 1, itemY + buttonOffset + 48, shadowColor);
        DrawLine(itemX + buttonOffset + menuItemWidth - 1, itemY + buttonOffset + 1, itemX + buttonOffset + menuItemWidth - 1, itemY + buttonOffset + 48, shadowColor);

        // Button text
        Color textColor = isSelected ? BLACK : Color{ 60, 60, 60, 255 };
        int textSize = isSelected ? 26 : 24;
        int textWidth = MeasureText(items[i], textSize);
        DrawText(items[i], itemX + (menuItemWidth - textWidth) / 2 + buttonOffset, itemY + 13 + buttonOffset, textSize, textColor);
    }

    // Decorative elements - Cooler animated game elements

    // Animated mini-spaceship (left) flying around the buttons
    float shipAngle = animTime * 30.0f; // Slow rotation
    int shipCenterX = 150;
    int shipCenterY = 320;
    int shipRadius = 80;
    int shipX = (int)(shipCenterX + cosf(shipAngle * DEG2RAD) * shipRadius);
    int shipY = (int)(shipCenterY + sinf(shipAngle * DEG2RAD) * shipRadius);

    // Mini-spaceship with thrust effect
    DrawTriangle(
        Vector2{ (float)shipX, (float)shipY - 8 },      // Tip
        Vector2{ (float)shipX - 6, (float)shipY + 6 },  // Left
        Vector2{ (float)shipX + 6, (float)shipY + 6 },  // Right
        Color{ 100, 100, 255, 255 }
    );
    // Thrust particles
    for (int i = 0; i < 3; i++) {
        int particleX = shipX + GetRandomValue(-2, 2);
        int particleY = shipY + 8 + i * 3;
        DrawCircle(particleX, particleY, 1, Color{ 255, 150, 0, (unsigned char)(200 - i * 60) });
    }

    // Large flying asteroids that fly around EVERYTHING (except buttons)
    for (int i = 0; i < 5; i++) {
        float astAngle = animTime * (20.0f + i * 5.0f) + i * 72.0f; // Different speeds
        float astRadius = 150.0f + i * 30.0f; // Different orbits

        // Center of elliptical orbit
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;

        // Elliptical orbits for more variation
        float radiusX = astRadius + 50.0f;
        float radiusY = astRadius;

        int astX = (int)(centerX + cosf(astAngle * DEG2RAD) * radiusX);
        int astY = (int)(centerY + sinf(astAngle * DEG2RAD) * radiusY);

        // Check if asteroid is in button area (adjusted for 5 buttons)
        bool inButtonArea = (astY > 160 && astY < 460 && astX > 200 && astX < SCREEN_WIDTH - 200);

        if (!inButtonArea) {
            float sizePulse = 1.0f + 0.4f * sinf(animTime * 2.0f + i * 1.2f);
            int asteroidSize = (int)((12 + i * 2) * sizePulse);

            Color astColors[] = {
                Color{200, 100, 50, 255},   // Orange
                Color{150, 75, 150, 255},   // Purple  
                Color{100, 150, 200, 255},  // Blue
                Color{200, 150, 100, 255},  // Brown
                Color{150, 200, 150, 255}   // Green
            };

            // Asteroid with rotation
            float rotAngle = animTime * 50.0f + i * 30.0f;

            // Main asteroid
            DrawCircle(astX, astY, (float)asteroidSize, astColors[i]);
            DrawCircleLines(astX, astY, (float)asteroidSize, BLACK);

            // Small details on asteroids
            int detailX = (int)(astX + cosf(rotAngle * DEG2RAD) * (float)asteroidSize * 0.3f);
            int detailY = (int)(astY + sinf(rotAngle * DEG2RAD) * (float)asteroidSize * 0.3f);

            // Safe color calculation for details
            unsigned char detailR = (unsigned char)SafeMax(0.0f, (float)(astColors[i].r - 30));
            unsigned char detailG = (unsigned char)SafeMax(0.0f, (float)(astColors[i].g - 30));
            unsigned char detailB = (unsigned char)SafeMax(0.0f, (float)(astColors[i].b - 30));

            DrawCircle(detailX, detailY, (float)(asteroidSize / 4), Color{ detailR, detailG, detailB, 255 });

            // Sparkle effect around asteroids
            if ((int)(animTime * 6.0f + i * 2.0f) % 4 == 0) {
                for (int sparkle = 0; sparkle < 3; sparkle++) {
                    int sparkleX = astX + GetRandomValue(-asteroidSize - 5, asteroidSize + 5);
                    int sparkleY = astY + GetRandomValue(-asteroidSize - 5, asteroidSize + 5);
                    DrawPixel(sparkleX, sparkleY, WHITE);
                }
            }
        }
    }

    // Animated projectiles flying between elements
    for (int i = 0; i < 4; i++) {
        float bulletTime = animTime * 2.0f + i * 1.5f;
        int startX = 200;
        int endX = SCREEN_WIDTH - 200;
        int bulletX = (int)(startX + (endX - startX) * (sinf(bulletTime) * 0.5f + 0.5f));
        int bulletY = 250 + i * 40 + (int)(sinf(bulletTime * 3.0f) * 20.0f);

        // Trail effect for projectiles
        for (int j = 0; j < 5; j++) {
            int trailX = bulletX - j * 8;
            unsigned char trailAlpha = (unsigned char)SafeMax(0.0f, (float)(255 - j * 50));
            DrawCircle(trailX, bulletY, 2.0f - (float)j * 0.3f, Color{ 255, 255, 100, trailAlpha });
        }

        DrawRectangle(bulletX - 2, bulletY - 4, 4, 8, BLACK);
    }

    // Power-up icons gently floating up and down
    int powerUpX = 80;
    int powerUpStartY = 480;
    const char* powerUpLabels[] = { "RAPID", "SHIELD", "LIFE" };
    Color powerUpColors[] = { RED, BLUE, GREEN };

    for (int i = 0; i < 3; i++) {
        float floatOffset = sinf(animTime * 2.0f + i * 2.0f) * 8.0f;
        int powerUpY = (int)(powerUpStartY + i * 35 + floatOffset);

        // Power-up box
        DrawRectangle(powerUpX - 15, powerUpY - 8, 30, 16, powerUpColors[i]);
        DrawRectangleLines(powerUpX - 15, powerUpY - 8, 30, 16, BLACK);

        // Power-up text
        int textWidth = MeasureText(powerUpLabels[i], 8);
        DrawText(powerUpLabels[i], powerUpX - textWidth / 2, powerUpY - 4, 8, WHITE);

        // Sparkle effect
        if ((int)(animTime * 5.0f + i) % 3 == 0) {
            DrawPixel(powerUpX + GetRandomValue(-20, 20), powerUpY + GetRandomValue(-15, 15), YELLOW);
        }
    }

    // Twinkling stars in background  
    for (int i = 0; i < 25; i++) {
        int starX = 30 + (i * 37) % (SCREEN_WIDTH - 60);
        int starY = 150 + (i * 23) % (SCREEN_HEIGHT - 180);

        // Stars avoid button area (adjusted for 5 buttons)
        if (starX > 200 && starX < SCREEN_WIDTH - 200 && starY > 150 && starY < 460) continue;

        float twinkle = sinf(animTime * 4.0f + (float)i * 0.8f);
        if (twinkle > 0.3f) {
            unsigned char alpha = (unsigned char)SafeMin(255.0f, SafeMax(0.0f, 100.0f + twinkle * 155.0f));
            unsigned char alphaHalf = (unsigned char)(alpha * 0.5f);
            DrawPixel(starX, starY, Color{ 255, 255, 255, alpha });
            DrawPixel(starX - 1, starY, Color{ 200, 200, 255, alphaHalf });
            DrawPixel(starX + 1, starY, Color{ 200, 200, 255, alphaHalf });
            DrawPixel(starX, starY - 1, Color{ 200, 200, 255, alphaHalf });
            DrawPixel(starX, starY + 1, Color{ 200, 200, 255, alphaHalf });
        }
    }

    // Control hints in MS Paint style
    DrawRectangle(20, SCREEN_HEIGHT - 80, SCREEN_WIDTH - 40, 60, Color{ 255, 255, 200, 255 });
    DrawRectangleLines(20, SCREEN_HEIGHT - 80, SCREEN_WIDTH - 40, 60, BLACK);

    const char* controls = "Use UP/DOWN arrows or W/S to navigate, ENTER to select, ESC to quit";
    int controlsWidth = MeasureText(controls, 16);
    DrawText(controls, SCREEN_WIDTH / 2 - controlsWidth / 2, SCREEN_HEIGHT - 60, 16, Color{ 80, 80, 80, 255 });

    // Version info
    const char* version = "v2.0 - Enhanced MS Paint Edition";
    int versionWidth = MeasureText(version, 12);
    DrawText(version, SCREEN_WIDTH / 2 - versionWidth / 2, SCREEN_HEIGHT - 35, 12, Color{ 120, 120, 120, 255 });

    // Blinking "Press any key" effect
    if ((int)(animTime * 3.0f) % 2 == 0) {
        const char* pressKey = "~ Made with love in MS Paint style ~";
        int pressKeyWidth = MeasureText(pressKey, 14);
        DrawText(pressKey, SCREEN_WIDTH / 2 - pressKeyWidth / 2, SCREEN_HEIGHT - 15, 14, Color{ 160, 80, 200, 255 });
    }
}

/**
 * Renders the in-game screen
 * Draws checkered background, all game objects, and UI overlay
 */
void UIRenderer::DrawInGame() const {
    // Draw background
    DrawCheckeredBackground();

    // Draw all game objects
    DrawGameObjects();

    // Draw UI overlay
    DrawGameUI();
}

/**
 * Renders all active game objects
 * Draws spaceship, projectiles, asteroids, and power-ups
 * Only renders objects that are currently active
 */
void UIRenderer::DrawGameObjects() const {
    // Draw spaceship (with all effects like shield, if active)
    player.Draw();

    // Draw all projectiles
    for (const auto& projectile : objectManager.GetProjectiles()) {
        if (projectile.IsActive()) {
            projectile.Draw();
        }
    }

    // Draw all asteroids
    for (const auto& asteroid : objectManager.GetAsteroids()) {
        if (asteroid.IsActive()) {
            asteroid.Draw();
        }
    }

    // Draw power-ups
    for (const auto& powerup : objectManager.GetPowerUps()) {
        if (powerup.IsActive()) {
            powerup.Draw();
        }
    }
}

/**
 * Renders the in-game UI overlay
 * Displays score, lives, level, high score, and status information
 * Shows shield timer, invulnerability status, and control hints
 */
void UIRenderer::DrawGameUI() const {
    // Main UI panel (top left)
    int mainUIWidth = 250;
    int mainUIHeight = 140;
    DrawRectangle(5, 5, mainUIWidth, mainUIHeight, Color{ 0, 0, 0, 150 });
    DrawRectangleLines(5, 5, mainUIWidth, mainUIHeight, WHITE);

    // Basic information
    DrawText(TextFormat("SCORE: %d", gameScore.GetScore()), 15, 15, 20, WHITE);
    DrawText(TextFormat("LIVES: %d", player.GetLives()), 15, 40, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", gameScore.GetLevel()), 15, 65, 20, WHITE);
    DrawText(TextFormat("HIGH: %d", gameScore.GetHighScore()), 15, 90, 20, YELLOW);

    // Item slot display (top right)
    DrawItemSlot();

    // Shield status display
    if (player.IsShieldActive()) {
        float timeRemaining = player.GetShieldTimeRemaining();
        DrawText(TextFormat("SHIELD: %.1fs", timeRemaining), 15, 115, 16, BLUE);
    }

    // Invulnerability status display
    if (player.IsInvulnerable()) {
        int yOffset = player.IsShieldActive() ? 135 : 115; // Adjust position based on shield status
        DrawText("INVULNERABLE", 15, yOffset, 16, YELLOW);
    }

    // Control hint (bottom right)
    DrawText("SHIFT: Use Item", SCREEN_WIDTH - 120, SCREEN_HEIGHT - 25, 12, LIGHTGRAY);
}

/**
 * Renders the power-up item slot display
 * Shows current item with color coding, name, and activation hint
 * Displays pulsing animation for active items or empty slot message
 */
void UIRenderer::DrawItemSlot() const {
    // Item slot position (top right)
    int slotX = SCREEN_WIDTH - 120;
    int slotY = 15;
    int slotSize = 80;

    // Slot background
    DrawRectangle(slotX, slotY, slotSize, slotSize, Color{ 40, 40, 40, 200 });
    DrawRectangleLines(slotX, slotY, slotSize, slotSize, WHITE);

    // Slot title
    DrawText("ITEM SLOT", slotX - 20, slotY - 15, 12, WHITE);

    // Display item content
    if (currentItem > 0) {
        // Item properties based on type
        const char* itemNames[] = { "", "RAPID", "SHIELD", "LIFE" };
        Color itemColors[] = { WHITE, RED, BLUE, GREEN };

        if (currentItem >= 1 && currentItem <= 3) {
            // Item background
            Color bgColor = itemColors[currentItem];
            bgColor.a = 150; // More visibility
            DrawRectangle(slotX + 5, slotY + 5, slotSize - 10, slotSize - 10, bgColor);

            // Item symbol/text (simple text since symbols can be problematic)
            const char* symbols[] = { "", "R", "S", "L" }; // Letters instead of symbols
            int symbolWidth = MeasureText(symbols[currentItem], 32);
            DrawText(symbols[currentItem],
                slotX + (slotSize - symbolWidth) / 2,
                slotY + 15, 32, WHITE);

            // Item name
            int nameWidth = MeasureText(itemNames[currentItem], 12);
            DrawText(itemNames[currentItem],
                slotX + (slotSize - nameWidth) / 2,
                slotY + 50, 12, WHITE);

            // Activation hint
            DrawText("SHIFT", slotX + (slotSize - MeasureText("SHIFT", 10)) / 2,
                slotY + 65, 10, YELLOW);

            // Pulsing border for active item
            static float pulseTimer = 0;
            pulseTimer += GetFrameTime();
            float pulse = 0.7f + 0.3f * sinf(pulseTimer * 5.0f);
            Color pulseColor = itemColors[currentItem];
            pulseColor.a = (unsigned char)(255 * pulse);
            DrawRectangleLines(slotX + 2, slotY + 2, slotSize - 4, slotSize - 4, pulseColor);
        }
    }
    else {
        // Empty slot
        DrawText("EMPTY", slotX + 20, slotY + 30, 12, GRAY);
        DrawText("Collect", slotX + 15, slotY + 45, 10, GRAY);
        DrawText("PowerUps", slotX + 12, slotY + 57, 10, GRAY);
    }

    // Strengthen slot border
    DrawRectangleLines(slotX - 1, slotY - 1, slotSize + 2, slotSize + 2, WHITE);
}

/**
 * Renders the game over screen
 * Displays final score, high score, and navigation options
 * Shows special message for new high scores
 */
void UIRenderer::DrawGameOver() const {
    // Dark background
    ClearBackground(BLACK);

    // Main text
    const char* gameOverText = "GAME OVER";
    int gameOverWidth = MeasureText(gameOverText, 60);
    DrawText(gameOverText, SCREEN_WIDTH / 2 - gameOverWidth / 2, SCREEN_HEIGHT / 2 - 100, 60, RED);

    // Score information
    const char* finalScore = TextFormat("FINAL SCORE: %d", gameScore.GetScore());
    int scoreWidth = MeasureText(finalScore, 30);
    DrawText(finalScore, SCREEN_WIDTH / 2 - scoreWidth / 2, SCREEN_HEIGHT / 2 - 20, 30, WHITE);

    const char* highScore = TextFormat("HIGH SCORE: %d", gameScore.GetHighScore());
    int highScoreWidth = MeasureText(highScore, 24);
    DrawText(highScore, SCREEN_WIDTH / 2 - highScoreWidth / 2, SCREEN_HEIGHT / 2 + 20, 24, YELLOW);

    // New high score?
    if (gameScore.GetScore() == gameScore.GetHighScore() && gameScore.GetScore() > 0) {
        const char* newRecord = "NEW HIGH SCORE!";
        int recordWidth = MeasureText(newRecord, 20);
        DrawText(newRecord, SCREEN_WIDTH / 2 - recordWidth / 2, SCREEN_HEIGHT / 2 + 60, 20, GREEN);
    }

    // Instructions
    const char* instruction1 = "Press ENTER to return to menu";
    int inst1Width = MeasureText(instruction1, 20);
    DrawText(instruction1, SCREEN_WIDTH / 2 - inst1Width / 2, SCREEN_HEIGHT / 2 + 100, 20, LIGHTGRAY);

    const char* instruction3 = "Press ESC to quit";
    int inst3Width = MeasureText(instruction3, 16);
    DrawText(instruction3, SCREEN_WIDTH / 2 - inst3Width / 2, SCREEN_HEIGHT / 2 + 150, 16, GRAY);
}

/**
 * Renders the options/controls menu screen
 * Displays comprehensive control instructions, power-up information, and tips
 * Organized in sections: Movement, Combat, Power-ups, Navigation, and Tips
 */
void UIRenderer::DrawOptionsMenu() const {
    // Controls menu background
    ClearBackground(DARKGRAY);
    DrawRectangle(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60, LIGHTGRAY);
    DrawRectangleLines(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60, BLACK);

    // Title
    const char* title = "CONTROLS";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 60, 40, BLACK);

    // Subtitle
    const char* subtitle = "Master the art of space flight";
    int subtitleWidth = MeasureText(subtitle, 16);
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, 110, 16, DARKGRAY);

    // Control categories
    int startY = 150;
    int lineHeight = 25;
    int currentY = startY;

    // Movement section
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

    // Combat section
    DrawText("COMBAT:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("SPACEBAR", 80, currentY, 16, BLACK);
    DrawText("- Fire projectiles", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    DrawText("SHIFT", 80, currentY, 16, BLACK);
    DrawText("- Use collected item", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight + 15;

    // Items section
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

    // Navigation section
    DrawText("NAVIGATION:", 60, currentY, 20, MAROON);
    currentY += 30;

    DrawText("TAB", 80, currentY, 16, BLACK);
    DrawText("- Return to main menu / Quit game", 220, currentY, 16, DARKGRAY);
    currentY += lineHeight;

    // Tips section
    DrawText("TIPS:", 60, currentY, 20, MAROON);
    currentY += 25;

    DrawText("• Destroy large asteroids to create smaller ones", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• Collect power-ups by flying over them", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• You are invulnerable for 3 seconds after respawning", 80, currentY, 14, DARKBLUE);
    currentY += 20;
    DrawText("• Power-ups spawn every 15 seconds or from large asteroids", 80, currentY, 14, DARKBLUE);

    // Return instruction
    const char* backText = "Press ENTER or ESC to return to main menu";
    int backWidth = MeasureText(backText, 18);
    DrawText(backText, SCREEN_WIDTH / 2 - backWidth / 2, SCREEN_HEIGHT - 80, 18, BLACK);

    // Decorative elements
    DrawRectangleLines(50, 140, SCREEN_WIDTH - 100, 2, BLACK);
    DrawRectangleLines(50, SCREEN_HEIGHT - 100, SCREEN_WIDTH - 100, 2, BLACK);
}

/**
 * Renders the name entry screen for new high scores
 * Features animated background, score display, and character-by-character input
 * Shows current name with fixed-width character boxes and blinking cursor
 * Includes comprehensive input instructions and celebration particle effects
 */
void UIRenderer::DrawNameEntryScreen() const {
    // Dark background with gradient
    ClearBackground(Color{ 20, 20, 40, 255 });

    // Main title
    const char* title = "NEW HIGH SCORE!";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 100, 50, GOLD);

    // Congratulations text
    const char* congratsText = "Congratulations! You achieved a new high score!";
    int congratsWidth = MeasureText(congratsText, 20);
    DrawText(congratsText, SCREEN_WIDTH / 2 - congratsWidth / 2, 170, 20, WHITE);

    // Display score
    const char* scoreText = TextFormat("Your Score: %d", stateManager.GetNameEntryScore());
    int scoreWidth = MeasureText(scoreText, 30);
    DrawText(scoreText, SCREEN_WIDTH / 2 - scoreWidth / 2, 220, 30, YELLOW);

    // Position in rankings
    const char* posText = TextFormat("Rank: #%d", stateManager.GetNameEntryPosition() + 1);
    int posWidth = MeasureText(posText, 25);
    DrawText(posText, SCREEN_WIDTH / 2 - posWidth / 2, 260, 25, LIME);

    // Name entry area
    DrawRectangle(SCREEN_WIDTH / 2 - 200, 320, 400, 80, Color{ 40, 40, 60, 200 });
    DrawRectangleLines(SCREEN_WIDTH / 2 - 200, 320, 400, 80, WHITE);

    // Name entry title
    const char* nameTitle = "Enter your name (max 5 characters):";
    int nameTitleWidth = MeasureText(nameTitle, 18);
    DrawText(nameTitle, SCREEN_WIDTH / 2 - nameTitleWidth / 2, 300, 18, LIGHTGRAY);

    // Current name with cursor
    std::string currentName = stateManager.GetPlayerName();

    // Fill name with underscores for better visualization
    std::string displayName = currentName;
    while (displayName.length() < 5) {
        displayName += "_";
    }

    // FIXED values for better display
    int nameSize = 40;
    int charWidth = 60;  // Fixed width per letter
    int totalWidth = charWidth * 5;  // 5 letters
    int nameX = SCREEN_WIDTH / 2 - totalWidth / 2;
    int nameY = 340;

    // Background for each letter - with fixed width
    for (int i = 0; i < 5; i++) {
        int charX = nameX + i * charWidth;

        // Letter background
        Color bgColor = (i < (int)currentName.length()) ? Color{ 60, 80, 100, 255 } : Color{ 30, 30, 50, 255 };
        DrawRectangle(charX, nameY - 5, charWidth - 2, nameSize + 10, bgColor);
        DrawRectangleLines(charX, nameY - 5, charWidth - 2, nameSize + 10, WHITE);

        // Letter - centered in field
        char charStr[2] = { displayName[i], '\0' };
        Color textColor = (i < (int)currentName.length()) ? WHITE : GRAY;
        int singleCharWidth = MeasureText(charStr, nameSize);
        int centeredX = charX + (charWidth - singleCharWidth) / 2;
        DrawText(charStr, centeredX, nameY, nameSize, textColor);
    }

    // Blinking cursor - adjusted to fixed positions
    if ((int)(stateManager.GetCursorBlinkTimer() * 2.0f) % 2 == 0 && currentName.length() < 5) {
        int cursorX = nameX + (int)currentName.length() * charWidth + charWidth / 2;
        DrawLine(cursorX, nameY, cursorX, nameY + nameSize, YELLOW);
    }

    // Instructions
    const char* instructions[] = {
        "Use A-Z and 0-9 to enter your name",
        "Use BACKSPACE to delete characters",
        "Use MINUS (-) for underscore",
        "Press ENTER to confirm (minimum 1 character)",
        "Press ESC to use default name (ANON_)"
    };

    for (int i = 0; i < 5; i++) {
        int instrWidth = MeasureText(instructions[i], 16);
        DrawText(instructions[i], SCREEN_WIDTH / 2 - instrWidth / 2, 450 + i * 25, 16, LIGHTGRAY);
    }

    // Show progress
    const char* progressText = TextFormat("Characters entered: %d/5", (int)currentName.length());
    int progressWidth = MeasureText(progressText, 14);
    DrawText(progressText, SCREEN_WIDTH / 2 - progressWidth / 2, 580, 14, GRAY);

    // Animated particles for celebration effect
    static float particleTimer = 0;
    particleTimer += GetFrameTime();

    for (int i = 0; i < 20; i++) {
        float angle = particleTimer * 50.0f + i * 18.0f;
        float radius = 100.0f + sin(particleTimer + i) * 30.0f;

        int particleX = (int)(SCREEN_WIDTH / 2 + cos(angle * DEG2RAD) * radius);
        int particleY = (int)(200 + sin(angle * DEG2RAD) * radius * 0.5f);

        Color particleColor = (i % 3 == 0) ? GOLD : ((i % 3 == 1) ? YELLOW : ORANGE);
        DrawCircle(particleX, particleY, 3, particleColor);
    }
}

/**
 * Renders the high score display screen
 * Shows top 10 high scores in a formatted table with rankings
 * Features special highlighting for top 3 positions (gold, silver, bronze)
 * Includes animated star background and return instructions
 */
void UIRenderer::DrawHighscoreScreen() const {
    // Elegant background
    ClearBackground(Color{ 15, 15, 30, 255 });

    // Title
    const char* title = "HIGH SCORES";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 50, 50, GOLD);

    // Subtitle
    const char* subtitle = "Hall of Fame - Top Pilots";
    int subtitleWidth = MeasureText(subtitle, 20);
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, 110, 20, LIGHTGRAY);

    // High score list
    if (highscoreManager) {
        const auto& highscores = highscoreManager->GetHighscores();

        // Table header
        int tableStartY = 160;
        int lineHeight = 40;

        // Header background
        DrawRectangle(SCREEN_WIDTH / 2 - 400, tableStartY - 10, 800, 35, Color{ 40, 40, 60, 150 });
        DrawRectangleLines(SCREEN_WIDTH / 2 - 400, tableStartY - 10, 800, 35, WHITE);

        // Header text
        DrawText("RANK", SCREEN_WIDTH / 2 - 350, tableStartY, 20, WHITE);
        DrawText("NAME", SCREEN_WIDTH / 2 - 200, tableStartY, 20, WHITE);
        DrawText("SCORE", SCREEN_WIDTH / 2 + 200, tableStartY, 20, WHITE);

        // High score entries
        for (size_t i = 0; i < highscores.size() && i < 10; i++) {
            int entryY = tableStartY + 40 + (int)i * lineHeight;

            // Alternating background colors
            Color bgColor = (i % 2 == 0) ? Color{ 25, 25, 45, 100 } : Color{ 35, 35, 55, 100 };
            DrawRectangle(SCREEN_WIDTH / 2 - 400, entryY - 5, 800, lineHeight - 5, bgColor);

            // Special highlighting for top 3
            Color textColor = WHITE;
            Color rankColor = WHITE;
            if (i == 0) { rankColor = GOLD; textColor = GOLD; }
            else if (i == 1) { rankColor = Color{ 192, 192, 192, 255 }; textColor = Color{ 192, 192, 192, 255 }; } // Silver
            else if (i == 2) { rankColor = Color{ 205, 127, 50, 255 }; textColor = Color{ 205, 127, 50, 255 }; } // Bronze

            // Rank
            const char* rankText = TextFormat("#%d", (int)i + 1);
            DrawText(rankText, SCREEN_WIDTH / 2 - 350, entryY, 24, rankColor);

            // Name
            DrawText(highscores[i].name.c_str(), SCREEN_WIDTH / 2 - 200, entryY, 24, textColor);

            // Score with formatting
            const char* scoreText = TextFormat("%d", highscores[i].score);
            int scoreWidth = MeasureText(scoreText, 24);
            DrawText(scoreText, SCREEN_WIDTH / 2 + 350 - scoreWidth, entryY, 24, textColor);

            // Decorative line after top 3
            if (i == 2) {
                DrawLine(SCREEN_WIDTH / 2 - 380, entryY + 35, SCREEN_WIDTH / 2 + 380, entryY + 35, GRAY);
            }
        }

        // If fewer than 10 entries are available
        if (highscores.size() < 10) {
            for (size_t i = highscores.size(); i < 10; i++) {
                int entryY = tableStartY + 40 + (int)i * lineHeight;

                Color bgColor = (i % 2 == 0) ? Color{ 25, 25, 45, 50 } : Color{ 35, 35, 55, 50 };
                DrawRectangle(SCREEN_WIDTH / 2 - 400, entryY - 5, 800, lineHeight - 5, bgColor);

                const char* rankText = TextFormat("#%d", (int)i + 1);
                DrawText(rankText, SCREEN_WIDTH / 2 - 350, entryY, 24, GRAY);
                DrawText("-----", SCREEN_WIDTH / 2 - 200, entryY, 24, GRAY);
                DrawText("---", SCREEN_WIDTH / 2 + 300, entryY, 24, GRAY);
            }
        }

        // Border around entire table
        DrawRectangleLines(SCREEN_WIDTH / 2 - 400, tableStartY - 10, 800, 450, WHITE);
    }

    // Instructions
    const char* instruction = "Press ENTER or ESC to return to main menu";
    int instrWidth = MeasureText(instruction, 20);
    DrawText(instruction, SCREEN_WIDTH / 2 - instrWidth / 2, SCREEN_HEIGHT - 80, 20, WHITE);

    // Additional info
    const char* info = "Destroy asteroids and collect power-ups to climb the rankings!";
    int infoWidth = MeasureText(info, 16);
    DrawText(info, SCREEN_WIDTH / 2 - infoWidth / 2, SCREEN_HEIGHT - 50, 16, LIGHTGRAY);

    // Animated stars in background
    static float starTimer = 0;
    starTimer += GetFrameTime();

    for (int i = 0; i < 30; i++) {
        float twinkle = sin(starTimer * 2.0f + (float)i * 0.5f);
        if (twinkle > 0.2f) {
            int starX = 50 + (i * 37) % (SCREEN_WIDTH - 100);
            int starY = 130 + (i * 23) % (SCREEN_HEIGHT - 200);

            // Avoid stars in table area
            if (starX > SCREEN_WIDTH / 2 - 420 && starX < SCREEN_WIDTH / 2 + 420 &&
                starY > 140 && starY < 620) continue;

            unsigned char alpha = (unsigned char)(100 + twinkle * 155);
            DrawPixel(starX, starY, Color{ 255, 255, 255, alpha });
        }
    }
}

/**
 * Renders the credits screen with scrolling animation
 * Features animated background, scrolling credits list, and special thanks
 * Includes particle effects, animated spaceships, and comprehensive development credits
 * All credits attributed to Adrian Kranyak with different roles and colors
 */
void UIRenderer::DrawCreditsScreen() const {
    // Animation time for scroll effects
    static float animTime = 0;
    animTime += GetFrameTime();

    // Elegant background with starfield
    ClearBackground(Color{ 10, 10, 25, 255 });

    // Animated stars in background
    for (int i = 0; i < 100; i++) {
        float twinkle = sin(animTime * 1.5f + (float)i * 0.3f);
        if (twinkle > 0.1f) {
            int starX = (i * 47) % SCREEN_WIDTH;
            int starY = (i * 23) % SCREEN_HEIGHT;

            unsigned char alpha = (unsigned char)(50 + twinkle * 100);
            Color starColor = (i % 4 == 0) ? BLUE : ((i % 4 == 1) ? WHITE : ((i % 4 == 2) ? YELLOW : PURPLE));
            starColor.a = alpha;
            DrawPixel(starX, starY, starColor);
        }
    }

    // Main title with glow effect
    const char* title = "CREDITS";
    int titleWidth = MeasureText(title, 60);
    float titleGlow = 1.0f + 0.3f * sin(animTime * 3.0f);

    // Title glow (multiple layers)
    for (int i = 0; i < 3; i++) {
        Color glowColor = GOLD;
        glowColor.a = (unsigned char)(30 - i * 10);
        DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2 - i, 50 - i, (int)(60 * titleGlow), glowColor);
    }
    DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 50, (int)(60 * titleGlow), GOLD);

    // Subtitle
    const char* subtitle = "The People Behind This Epic Space Adventure";
    int subtitleWidth = MeasureText(subtitle, 20);
    DrawText(subtitle, SCREEN_WIDTH / 2 - subtitleWidth / 2, 120, 20, LIGHTGRAY);

    // Credits container (larger, scrollable area)
    int containerX = SCREEN_WIDTH / 2 - 500;
    int containerY = 160;
    int containerWidth = 1000;
    int containerHeight = 450;
    DrawRectangle(containerX, containerY, containerWidth, containerHeight, Color{ 20, 20, 40, 200 });
    DrawRectangleLines(containerX, containerY, containerWidth, containerHeight, GOLD);

    // Scrolling credits - vertically scrolling
    float scrollSpeed = 30.0f; // Pixels per second
    float totalContentHeight = 18 * 25 + 6 * 25 + 100; // Credits + Thanks + Padding
    float scrollY = fmod(animTime * scrollSpeed, totalContentHeight + containerHeight);

    int startY = (int)(containerY + containerHeight - scrollY);
    int lineHeight = 25;
    int currentY = startY;

    // Credits entries
    struct CreditEntry {
        const char* role;
        const char* name;
        Color roleColor;
        Color nameColor;
    };

    CreditEntry credits[] = {
        {"GAME DIRECTOR", "Adrian Kranyak", GOLD, WHITE},
        {"LEAD PROGRAMMER", "Adrian Kranyak", Color{100, 255, 100, 255}, WHITE},
        {"GAME DESIGNER", "Adrian Kranyak", Color{100, 150, 255, 255}, WHITE},
        {"GRAPHICS PROGRAMMER", "Adrian Kranyak", Color{255, 100, 255, 255}, WHITE},
        {"PHYSICS PROGRAMMER", "Adrian Kranyak", Color{255, 215, 0, 255}, WHITE},
        {"UI/UX DESIGNER", "Adrian Kranyak", Color{255, 150, 100, 255}, WHITE},
        {"AUDIO INTEGRATION", "Adrian Kranyak", Color{150, 255, 150, 255}, WHITE},
        {"COLLISION SYSTEM", "Adrian Kranyak", Color{150, 150, 255, 255}, WHITE},
        {"GAMEPLAY MECHANICS", "Adrian Kranyak", Color{255, 255, 100, 255}, WHITE},
        {"POWER-UP SYSTEM", "Adrian Kranyak", Color{255, 100, 150, 255}, WHITE},
        {"HIGHSCORE SYSTEM", "Adrian Kranyak", Color{100, 255, 255, 255}, WHITE},
        {"MENU SYSTEM", "Adrian Kranyak", Color{200, 100, 255, 255}, WHITE},
        {"VISUAL EFFECTS", "Adrian Kranyak", Color{255, 200, 100, 255}, WHITE},
        {"PARTICLE SYSTEMS", "Adrian Kranyak", Color{100, 200, 255, 255}, WHITE},
        {"ANIMATION SYSTEM", "Adrian Kranyak", Color{255, 150, 200, 255}, WHITE},
        {"GAME BALANCE", "Adrian Kranyak", Color{150, 255, 200, 255}, WHITE},
        {"QUALITY ASSURANCE", "Adrian Kranyak", Color{200, 255, 150, 255}, WHITE},
        {"PROJECT MANAGEMENT", "Adrian Kranyak", Color{255, 200, 200, 255}, WHITE}
    };

    int numCredits = sizeof(credits) / sizeof(credits[0]);

    // Draw credits (only if visible in container)
    for (int i = 0; i < numCredits; i++) {
        if (currentY >= containerY && currentY <= containerY + containerHeight - lineHeight) {
            // Role (left, but within container)
            DrawText(credits[i].role, containerX + 50, currentY, 16, credits[i].roleColor);

            // Separator dots (shorter)
            DrawText("........................", SCREEN_WIDTH / 2 - 80, currentY, 16, GRAY);

            // Name (right, but within container)
            int nameWidth = MeasureText(credits[i].name, 16);
            DrawText(credits[i].name, containerX + containerWidth - nameWidth - 50, currentY, 16, credits[i].nameColor);

            // Shimmer effect for Adrian's name
            if (strcmp(credits[i].name, "Adrian Kranyak") == 0 && (int)(animTime * 4.0f + i) % 8 == 0) {
                Color shimmer = YELLOW;
                shimmer.a = 100;
                DrawText(credits[i].name, containerX + containerWidth - nameWidth - 49, currentY + 1, 16, shimmer);
            }
        }
        currentY += lineHeight;
    }

    // Space before special thanks
    currentY += 40;

    // Special thanks title
    if (currentY >= containerY && currentY <= containerY + containerHeight - lineHeight) {
        const char* thanksTitle = "SPECIAL THANKS";
        int thanksTitleWidth = MeasureText(thanksTitle, 20);
        DrawText(thanksTitle, SCREEN_WIDTH / 2 - thanksTitleWidth / 2, currentY, 20, GOLD);
    }
    currentY += 30;

    // Special thanks list
    const char* specialThanks[] = {
        "Raylib - For making a cheat sheet that felt like ragebait",
        "Classic Atari Asteroids - For the timeless inspiration",
        "Redbull - for letting me stay awake",
        "Claude Ai - for helping me with design, because I am a programmer and dont understand colors",
        "GitHub - For version control and code management",
        "Visual Studio - For the excellent development environment",
        "Valorant - to have fun after hours of programming",
        "And you - for playing this game :)"
    };

    int numThanks = sizeof(specialThanks) / sizeof(specialThanks[0]);
    for (int i = 0; i < numThanks; i++) {
        if (currentY >= containerY && currentY <= containerY + containerHeight - lineHeight) {
            int thanksWidth = MeasureText(specialThanks[i], 14);
            DrawText(specialThanks[i], SCREEN_WIDTH / 2 - thanksWidth / 2, currentY, 14, LIGHTGRAY);
        }
        currentY += 25;
    }

    // Footer (outside container, fixed at screen edge)
    const char* copyright = "© 2025 Adrian Kranyak - All Rights Reserved";
    int copyrightWidth = MeasureText(copyright, 12);
    DrawText(copyright, SCREEN_WIDTH / 2 - copyrightWidth / 2, SCREEN_HEIGHT - 100, 12, GRAY);

    const char* version = "Asteroids Enhanced Edition v2.0 - MS Paint Style";
    int versionWidth = MeasureText(version, 14);
    DrawText(version, SCREEN_WIDTH / 2 - versionWidth / 2, SCREEN_HEIGHT - 80, 14, Color{ 150, 150, 150, 255 });

    const char* engine = "Powered by Raylib & C++";
    int engineWidth = MeasureText(engine, 12);
    DrawText(engine, SCREEN_WIDTH / 2 - engineWidth / 2, SCREEN_HEIGHT - 60, 12, Color{ 100, 150, 200, 255 });

    // Animated spaceships (outside container)
    for (int i = 0; i < 3; i++) {
        float shipAngle = animTime * (30.0f + i * 10.0f) + i * 120.0f;
        float shipRadius = 150.0f + i * 50.0f;

        int shipX = (int)(SCREEN_WIDTH / 2 + cos(shipAngle * DEG2RAD) * shipRadius);
        int shipY = (int)(SCREEN_HEIGHT / 2 + sin(shipAngle * DEG2RAD) * shipRadius * 0.3f);

        // Avoid collision with credits container
        if (shipX > containerX && shipX < containerX + containerWidth &&
            shipY > containerY && shipY < containerY + containerHeight) continue;

        Color shipColor = (i == 0) ? Color{ 100, 150, 255, 180 } :
            (i == 1) ? Color{ 255, 100, 150, 180 } :
            Color{ 150, 255, 100, 180 };

        // Mini-spaceship
        DrawTriangle(
            Vector2{ (float)shipX, (float)shipY - 6 },      // Tip
            Vector2{ (float)shipX - 4, (float)shipY + 4 },  // Left
            Vector2{ (float)shipX + 4, (float)shipY + 4 },  // Right
            shipColor
        );

        // Thrust particles
        for (int j = 0; j < 2; j++) {
            int particleX = shipX + GetRandomValue(-1, 1);
            int particleY = shipY + 6 + j * 2;
            Color particleColor = Color{ 255, (unsigned char)(150 + j * 50), 0, (unsigned char)(150 - j * 50) };
            DrawCircle(particleX, particleY, 1, particleColor);
        }
    }

    // Instructions for returning
    const char* instruction = "Press any key to return to main menu";
    int instrWidth = MeasureText(instruction, 18);

    // Blinking text
    if ((int)(animTime * 3.0f) % 2 == 0) {
        DrawText(instruction, SCREEN_WIDTH / 2 - instrWidth / 2, SCREEN_HEIGHT - 30, 18, WHITE);
    }
}