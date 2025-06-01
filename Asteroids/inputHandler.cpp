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
    }
}

void InputHandler::HandleMainMenuInput() {
    // Navigation im Hauptmenü
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection - 1 + 3) % 3);
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection + 1) % 3);
    }

    // Auswahl bestätigen
    if (IsKeyPressed(KEY_ENTER)) {
        switch (stateManager.GetMenuSelection()) {
        case 0: // START GAME
            stateManager.SetState(IN_GAME);
            break;
        case 1: // CONTROLS
            stateManager.SetState(OPTIONS);
            break;
        case 2: // QUIT
            game.SetGameRunning(false); // Spiel beenden
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

    // Schießen - KORRIGIERT: Projektile spawnen vor der Spitze
    if (IsKeyDown(KEY_SPACE) && projectileCooldown <= 0) {
        // Schuss-Position an der Spitze des Raumschiffs berechnen
        Vector2 playerPos = player.GetPosition();
        float playerRotation = player.GetRotation();
        float radian = playerRotation * DEG2RAD;

        // Projektil etwas vor der Raumschiff-Spitze spawnen
        Vector2 firePosition = {
            playerPos.x + cosf(radian) * 20.0f,  // 20 Pixel vor der Spitze
            playerPos.y + sinf(radian) * 20.0f
        };

        objectManager.FireProjectile(firePosition, playerRotation);

        // Schuss-Sound abspielen (über Game-Klasse)
        game.PlayShootSound();

        // Debug: Schuss-Info
        if (hasRapid) {
            printf("Rapid fire shot! Remaining: %d\n", amountRapid);
        }

        projectileCooldown = hasRapid ? 0.05f : 0.2f; // Schneller schießen wenn Rapid Fire aktiv

        if (hasRapid) {
            amountRapid--;
            if (amountRapid <= 0) {
                hasRapid = false; // Rapid Fire beenden wenn aufgebraucht
                printf("Rapid fire depleted!\n");
            }
        }
    }

    // Item verwenden
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        UseItem();
    }

    // Pause-Funktion (optional)
    if (IsKeyPressed(KEY_P)) {
        // TODO: Pause implementieren
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

    // Schneller Neustart (optional)
    if (IsKeyPressed(KEY_R)) {
        // TODO: Direkter Neustart implementieren
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

void InputHandler::UseItem() {
    auto& player = objectManager.GetPlayer();

    // Debug-Ausgabe
    printf("UseItem called! currentItem = %d\n", currentItem);

    switch (currentItem) {
    case 0: // Kein Item
        printf("No item to use\n");
        break;

    case 1: // Rapid Fire
        printf("Activating RAPID FIRE!\n");
        hasRapid = true;
        amountRapid = 50; // 50 Schüsse mit Rapid Fire
        currentItem = 0; // Item aufgebraucht
        printf("hasRapid = %s, amountRapid = %d\n", hasRapid ? "true" : "false", amountRapid);
        break;

    case 2: // Shield
        printf("Activating SHIELD!\n");
        hasShield = true;
        player.ActivateShield(); // Aktiviere visuelles Schild
        currentItem = 0; // Item aufgebraucht
        printf("hasShield = %s\n", hasShield ? "true" : "false");
        break;

    case 3: // Extra Life (wird normalerweise sofort angewendet)
        printf("Adding extra life\n");
        player.AddLife();
        currentItem = 0; // Item aufgebraucht
        break;

    default:
        printf("Unknown item type: %d\n", currentItem);
        currentItem = 0;
        break;
    }
}