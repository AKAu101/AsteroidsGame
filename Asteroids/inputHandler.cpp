#include "inputhandler.h"
#include "objectManager.h"
#include "globals.h"
#include "game.h" 

/**
 * Constructor for InputHandler class
 * Initializes all references to game systems and shared state variables
 * Sets up dependencies for input handling across different game states
 * @param gameRef Reference to the main game instance
 * @param stateMgr Reference to the game state manager
 * @param objMgr Reference to the object manager
 * @param projCooldown Reference to projectile cooldown timer
 * @param item Reference to current item type
 * @param rapid Reference to rapid fire status flag
 * @param rapidAmount Reference to remaining rapid fire shots
 * @param shield Reference to shield status flag
 */
InputHandler::InputHandler(Game& gameRef, GameStateManager& stateMgr, ObjectManager& objMgr,
    float& projCooldown, int& item, bool& rapid,
    int& rapidAmount, bool& shield) :
    game(gameRef),
    stateManager(stateMgr),
    objectManager(objMgr),
    projectileCooldown(projCooldown),
    currentItem(item),
    hasRapid(rapid),
    amountRapid(rapidAmount),
    hasShield(shield) {
}

/**
 * Main input handling dispatcher method
 * Routes input handling to appropriate state-specific methods
 * Called every frame to process user input based on current game state
 * @param deltaTime Time elapsed since last frame in seconds
 */
void InputHandler::HandleInput(float deltaTime) {
    switch (stateManager.GetCurrentState()) {
    case MAIN_MENU:
        HandleMainMenuInput();
        break;
    case IN_GAME:
        HandleGameInput(deltaTime);
        break;
    case OPTIONS:
        HandleOptionsInput();
        break;
    case GAME_OVER:
        HandleGameOverInput();
        break;
    case HIGHSCORE_ENTRY:
        HandleNameEntryInput();
        break;
    case HIGHSCORE_DISPLAY:
        HandleHighscoreDisplayInput();
        break;
    case CREDITS:
        HandleCreditsInput();
        break;
    }
}

/**
 * Handles input for the main menu navigation
 * Processes menu selection with UP/DOWN keys and confirms with ENTER
 * Supports 5 menu options: Start Game, Controls, Highscores, Credits, Quit
 * ESC key provides alternative quit option
 */
void InputHandler::HandleMainMenuInput() {
    // Navigation in main menu (now 5 options)
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection - 1 + 5) % 5);
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection + 1) % 5);
    }

    // Confirm selection
    if (IsKeyPressed(KEY_ENTER)) {
        switch (stateManager.GetMenuSelection()) {
        case 0: // START GAME
            game.ResetGame(); // IMPORTANT: Reset game before starting!
            stateManager.SetState(IN_GAME);
            break;
        case 1: // CONTROLS
            stateManager.SetState(OPTIONS);
            break;
        case 2: // HIGHSCORES
            stateManager.SetState(HIGHSCORE_DISPLAY);
            break;
        case 3: // CREDITS
            stateManager.SetState(CREDITS);
            break;
        case 4: // QUIT
            game.SetGameRunning(false);
            break;
        }
    }

    // Alternative: ESC to quit
    if (IsKeyPressed(KEY_ESCAPE)) {
        game.SetGameRunning(false);
    }
}

/**
 * Handles all gameplay input during active game state
 * Processes player movement, rotation, shooting, item usage, and menu access
 * Manages rapid fire mechanics and cooldown timers
 * @param deltaTime Time elapsed since last frame for smooth movement
 */
void InputHandler::HandleGameInput(float deltaTime) {
    auto& player = objectManager.GetPlayer();

    // Movement controls
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        player.StartThrust();
        player.ApplyThrust(deltaTime);
    }
    else {
        player.StopThrust();
    }

    // Rotation
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.Rotate(-1, deltaTime);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.Rotate(1, deltaTime);
    }

    // Shooting
    if (IsKeyDown(KEY_SPACE) && projectileCooldown <= 0) {
        Vector2 playerPos = player.GetPosition();
        float playerRotation = player.GetRotation();
        float radian = playerRotation * DEG2RAD;

        Vector2 firePosition = {
            playerPos.x + cosf(radian) * 20.0f,
            playerPos.y + sinf(radian) * 20.0f
        };

        objectManager.FireProjectile(firePosition, playerRotation);
        game.PlayShootSound();

        if (hasRapid) {
            printf("Rapid fire shot! Remaining: %d\n", amountRapid);
        }

        projectileCooldown = hasRapid ? 0.05f : 0.2f;

        if (hasRapid) {
            amountRapid--;
            if (amountRapid <= 0) {
                hasRapid = false;
                printf("Rapid fire depleted!\n");
            }
        }
    }

    // Use item
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        UseItem();
    }

    // Return to main menu
    if (IsKeyPressed(KEY_TAB)) {
        stateManager.SetState(MAIN_MENU);
    }
}

/**
 * Handles input for the game over screen
 * Provides options to return to menu, quit game, or quickly restart
 * ENTER returns to main menu, ESC quits, R provides quick restart
 */
void InputHandler::HandleGameOverInput() {
    // Return to main menu
    if (IsKeyPressed(KEY_ENTER)) {
        stateManager.SetState(MAIN_MENU);
    }

    // Alternative: ESC to quit
    if (IsKeyPressed(KEY_ESCAPE)) {
        game.SetGameRunning(false);
    }

    // Quick restart
    if (IsKeyPressed(KEY_R)) {
        game.ResetGame(); // IMPORTANT: Reset game on quick restart!
        stateManager.SetState(IN_GAME);
    }
}

/**
 * Handles input for the options/controls screen
 * Currently provides basic navigation placeholder and return to menu
 * Future scroll functionality can be implemented here
 */
void InputHandler::HandleOptionsInput() {
    // Navigation in controls
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        // Scroll function could be implemented here
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        // Scroll function could be implemented here
    }

    // Return to main menu
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

/**
 * Handles input for the credits screen
 * Returns to main menu on any of the specified key presses
 * Provides multiple exit options for user convenience
 */
void InputHandler::HandleCreditsInput() {
    // Return to main menu on any key
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) ||
        IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_BACKSPACE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

/**
 * Handles input for highscore name entry
 * Processes character input (A-Z, 0-9, underscore), backspace editing
 * Validates name completion and handles confirmation or cancellation
 * Supports 5-character names with automatic padding
 */
void InputHandler::HandleNameEntryInput() {
    std::string currentName = stateManager.GetPlayerName();

    // Enter letters A-Z
    for (int key = KEY_A; key <= KEY_Z; key++) {
        if (IsKeyPressed(key) && currentName.length() < 5) {
            char letter = 'A' + (key - KEY_A);
            currentName += letter;
            stateManager.SetPlayerName(currentName);
        }
    }

    // Enter numbers 0-9
    for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
        if (IsKeyPressed(key) && currentName.length() < 5) {
            char number = '0' + (key - KEY_ZERO);
            currentName += number;
            stateManager.SetPlayerName(currentName);
        }
    }

    // Underscore and dash
    if (IsKeyPressed(KEY_MINUS) && currentName.length() < 5) {
        currentName += '_';
        stateManager.SetPlayerName(currentName);
    }

    // Backspace - delete last character
    if (IsKeyPressed(KEY_BACKSPACE) && !currentName.empty()) {
        currentName.pop_back();
        stateManager.SetPlayerName(currentName);
    }

    // Enter - confirm name (only if at least 1 character entered)
    if (IsKeyPressed(KEY_ENTER) && stateManager.IsNameComplete()) {
        // Pad name with underscores if shorter than 5 characters
        while (currentName.length() < 5) {
            currentName += '_';
        }
        stateManager.SetPlayerName(currentName);

        // Add highscore through Game class
        game.AddHighscoreEntry(currentName, stateManager.GetNameEntryScore());
        stateManager.SetState(HIGHSCORE_DISPLAY);
    }

    // ESC - cancel (use default name)
    if (IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetPlayerName("ANON_");
        game.AddHighscoreEntry("ANON_", stateManager.GetNameEntryScore());
        stateManager.SetState(HIGHSCORE_DISPLAY);
    }
}

/**
 * Handles input for the highscore display screen
 * Simple input handling that returns to main menu on ENTER or ESC
 */
void InputHandler::HandleHighscoreDisplayInput() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

/**
 * Activates and uses the currently held power-up item
 * Handles different item types: Rapid Fire, Shield, and Extra Life
 * Resets current item to 0 after use
 * Updates relevant game state flags and counters
 */
void InputHandler::UseItem() {
    auto& player = objectManager.GetPlayer();

    printf("UseItem called! currentItem = %d\n", currentItem);

    switch (currentItem) {
    case 0: // No item
        printf("No item to use\n");
        break;

    case 1: // Rapid Fire
        printf("Activating RAPID FIRE!\n");
        hasRapid = true;
        amountRapid = 50;
        currentItem = 0;
        break;

    case 2: // Shield
        printf("Activating SHIELD!\n");
        hasShield = true;
        player.ActivateShield();
        currentItem = 0;
        break;

    case 3: // Extra Life
        printf("Adding extra life\n");
        player.AddLife();
        currentItem = 0;
        break;

    default:
        currentItem = 0;
        break;
    }
}