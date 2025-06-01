#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "gamestate.h"
#include "objectmanager.h"

class Game;
class ObjectManager;

class InputHandler {
private:
    Game& game;
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
    void HandleHighscoreDisplayInput();
    void HandleNameEntryInput();
    void HandleCreditsInput();  // Neue Methode für Credits

public:
    InputHandler(Game& game, GameStateManager& stateMgr, ObjectManager& objMgr,
        float& projCooldown, int& item, bool& rapid, int& rapidAmount, bool& shield);

    void HandleInput(float deltaTime);

private:
    void UseItem();
};

#endif