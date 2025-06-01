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
    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection - 1 + 3) % 3);
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        int selection = stateManager.GetMenuSelection();
        stateManager.SetMenuSelection((selection + 1) % 3);
    }

    // Selection
    if (IsKeyPressed(KEY_ENTER)) {
        switch (stateManager.GetMenuSelection()) {
        case 0: stateManager.SetState(IN_GAME); break;
        case 1: stateManager.SetState(OPTIONS); break;
        case 2: game.SetGameRunning(false); break;
        }
    }
}

void InputHandler::HandleGameInput(float deltaTime) {
    auto& player = objectManager.GetPlayer();

    // Movement
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
        objectManager.FireProjectile(player.GetPosition(), player.GetRotation());
        projectileCooldown = hasRapid ? 0.05f : 0.2f;
        if (hasRapid) amountRapid--;
    }

    // Item usage
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        UseItem();
    }
}

void InputHandler::HandleGameOverInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        stateManager.SetState(MAIN_MENU);
    }
}

void InputHandler::HandleOptionsInput() {
    // TODO: Handle options input
}

void InputHandler::UseItem() {
    switch (currentItem) {
    case 1: // Rapid fire
        hasRapid = true;
        amountRapid = 10;
        break;
    case 2: // Shield
        hasShield = true;
        break;
    }
    currentItem = 0;
}