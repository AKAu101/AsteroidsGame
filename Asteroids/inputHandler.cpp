#include "inputhandler.h"
#include "objectManager.h"
#include "globals.h"
#include "game.h" 

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

void InputHandler::HandleMainMenuInput() {
    // Navigation im Hauptmenü (jetzt 5 Optionen)
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection - 1 + 5) % 5);
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection + 1) % 5);
    }

    // Auswahl bestätigen
    if (IsKeyPressed(KEY_ENTER)) {
        switch (stateManager.GetMenuSelection()) {
        case 0: // START GAME
            game.ResetGame(); // WICHTIG: Spiel zurücksetzen vor dem Start!
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

    // Alternative: ESC zum Beenden
    if (IsKeyPressed(KEY_ESCAPE)) {
        game.SetGameRunning(false);
    }
}

void InputHandler::HandleGameInput(float deltaTime) {
    auto& player = objectManager.GetPlayer();

    // Bewegungssteuerung
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

    // Schießen
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

    // Item verwenden
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        UseItem();
    }

    // Zurück zum Hauptmenü
    if (IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

void InputHandler::HandleGameOverInput() {
    // Zurück zum Hauptmenü
    if (IsKeyPressed(KEY_ENTER)) {
        stateManager.SetState(MAIN_MENU);
    }

    // Alternativ: ESC zum Beenden
    if (IsKeyPressed(KEY_ESCAPE)) {
        game.SetGameRunning(false);
    }

    // Schneller Neustart
    if (IsKeyPressed(KEY_R)) {
        game.ResetGame(); // WICHTIG: Spiel zurücksetzen beim schnellen Neustart!
        stateManager.SetState(IN_GAME);
    }
}

void InputHandler::HandleOptionsInput() {
    // Navigation in den Controls
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        // Scroll-Funktion könnte hier implementiert werden
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        // Scroll-Funktion könnte hier implementiert werden
    }

    // Zurück zum Hauptmenü
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

void InputHandler::HandleCreditsInput() {
    // Zurück zum Hauptmenü bei beliebiger Taste
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) ||
        IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_BACKSPACE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

void InputHandler::HandleNameEntryInput() {
    std::string currentName = stateManager.GetPlayerName();

    // Buchstaben A-Z eingeben
    for (int key = KEY_A; key <= KEY_Z; key++) {
        if (IsKeyPressed(key) && currentName.length() < 5) {
            char letter = 'A' + (key - KEY_A);
            currentName += letter;
            stateManager.SetPlayerName(currentName);
        }
    }

    // Zahlen 0-9 eingeben
    for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
        if (IsKeyPressed(key) && currentName.length() < 5) {
            char number = '0' + (key - KEY_ZERO);
            currentName += number;
            stateManager.SetPlayerName(currentName);
        }
    }

    // Unterstrich und Bindestrich
    if (IsKeyPressed(KEY_MINUS) && currentName.length() < 5) {
        currentName += '_';
        stateManager.SetPlayerName(currentName);
    }

    // Backspace - letztes Zeichen löschen
    if (IsKeyPressed(KEY_BACKSPACE) && !currentName.empty()) {
        currentName.pop_back();
        stateManager.SetPlayerName(currentName);
    }

    // Enter - Name bestätigen (nur wenn mindestens 1 Zeichen eingegeben)
    if (IsKeyPressed(KEY_ENTER) && stateManager.IsNameComplete()) {
        // Name mit Unterstrichen auffüllen falls kürzer als 5 Zeichen
        while (currentName.length() < 5) {
            currentName += '_';
        }
        stateManager.SetPlayerName(currentName);

        // Highscore hinzufügen über Game-Klasse
        game.AddHighscoreEntry(currentName, stateManager.GetNameEntryScore());
        stateManager.SetState(HIGHSCORE_DISPLAY);
    }

    // ESC - Abbrechen (Standardname verwenden)
    if (IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetPlayerName("ANON_");
        game.AddHighscoreEntry("ANON_", stateManager.GetNameEntryScore());
        stateManager.SetState(HIGHSCORE_DISPLAY);
    }
}

void InputHandler::HandleHighscoreDisplayInput() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        stateManager.SetState(MAIN_MENU);
    }
}

void InputHandler::UseItem() {
    auto& player = objectManager.GetPlayer();

    printf("UseItem called! currentItem = %d\n", currentItem);

    switch (currentItem) {
    case 0: // Kein Item
        printf("No item to use\n");
        break;

    case 1: // Rapid Fire
        printf("Activating RAPID FIRE!\n");
        hasRapid = true;
        amountRapid = 50;
        currentItem = 0;
        printf("hasRapid = %s, amountRapid = %d\n", hasRapid ? "true" : "false", amountRapid);
        break;

    case 2: // Shield
        printf("Activating SHIELD!\n");
        hasShield = true;
        player.ActivateShield();
        currentItem = 0;
        printf("hasShield = %s\n", hasShield ? "true" : "false");
        break;

    case 3: // Extra Life
        printf("Adding extra life\n");
        player.AddLife();
        currentItem = 0;
        break;

    default:
        printf("Unknown item type: %d\n", currentItem);
        currentItem = 0;
        break;
    }
}