#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "gamestate.h"
#include "score.h"
#include "spaceship.h"
#include "objectmanager.h"

class UIRenderer {
private:
    GameStateManager& stateManager;
    Score& gameScore;
    Spaceship& player;
    ObjectManager& objectManager;
    int& currentItem; // Referenz auf currentItem aus Game

public:
    UIRenderer(GameStateManager& stateMgr, Score& score, Spaceship& ship, ObjectManager& objMgr, int& currentItemRef);

    void DrawCurrentState() const;

private:
    void DrawCheckeredBackground() const;
    void DrawMainMenu() const;
    void DrawInGame() const;
    void DrawGameOver() const;
    void DrawGameObjects() const;
    void DrawGameUI() const;
    void DrawOptionsMenu() const;
    void DrawItemSlot() const;
};

#endif