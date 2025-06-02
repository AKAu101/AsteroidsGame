#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "gamestate.h"
#include "objectmanager.h"

class Game;
class ObjectManager;

class InputHandler {
private:
    Game& game;                         // Reference to main game instance
    GameStateManager& stateManager;     // Reference to game state manager
    ObjectManager& objectManager;       // Reference to object manager
    float& projectileCooldown;          // Reference to projectile firing cooldown timer
    int& currentItem;                   // Reference to currently held power-up item
    bool& hasRapid;                     // Reference to rapid fire status flag
    int& amountRapid;                   // Reference to remaining rapid fire shots
    bool& hasShield;                    // Reference to shield protection status

    void HandleMainMenuInput();
    void HandleOptionsInput();
    void HandleGameInput(float deltaTime);
    void HandleGameOverInput();
    void HandleHighscoreDisplayInput();
    void HandleNameEntryInput();
    void HandleCreditsInput();

public:
    InputHandler(Game& game /* Reference to main game instance */,
        GameStateManager& stateMgr /* Reference to state manager */,
        ObjectManager& objMgr /* Reference to object manager */,
        float& projCooldown /* Reference to projectile cooldown timer */,
        int& item /* Reference to current power-up item */,
        bool& rapid /* Reference to rapid fire status */,
        int& rapidAmount /* Reference to remaining rapid fire shots */,
        bool& shield /* Reference to shield status */);

    void HandleInput(float deltaTime);

private:
    void UseItem();
};

#endif