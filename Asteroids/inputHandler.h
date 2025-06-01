#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "gamestate.h"
#include "objectmanager.h"

class InputHandler {
private:
    GameStateManager& stateManager;
    ObjectManager& objectManager;
    float& projectileCooldown;
    int& currentItem;
    bool& hasRapid;
    int& amountRapid;
    bool& hasShield;

    void HandleMainMenuInput();
    void HandleOptionsInput();
    void HandleGameInput(float deltaTime);
    void HandleGameOverInput();

public:
    InputHandler(GameStateManager& stateMgr, ObjectManager& objMgr,
        float& projCooldown, int& item, bool& rapid, int& rapidAmount, bool& shield);

    void HandleInput(float deltaTime);

private:
    void UseItem();
};

#endif