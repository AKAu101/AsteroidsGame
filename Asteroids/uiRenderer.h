#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "gamestate.h"
#include "score.h"
#include "spaceship.h"
#include "objectmanager.h"

class UIRenderer {
private:
    GameStateManager& stateManager;     // Reference to game state manager for state-based rendering
    Score& gameScore;                   // Reference to score system for displaying points and levels
    Spaceship& player;                  // Reference to player spaceship for status information
    ObjectManager& objectManager;       // Reference to object manager for rendering game objects
    int& currentItem;                   // Reference to currently held power-up item
    class HighscoreManager* highscoreManager = nullptr; // Pointer to high score manager (optional)

public:
    UIRenderer(GameStateManager& stateMgr /* Reference to state manager */,
        Score& score /* Reference to score system */,
        Spaceship& ship /* Reference to player spaceship */,
        ObjectManager& objMgr /* Reference to object manager */,
        int& currentItemRef /* Reference to current power-up item */);

    void DrawCurrentState() const;
    void SetHighscoreManager(class HighscoreManager* hsMgr) {
        highscoreManager = hsMgr;
    }

private:
    void DrawCheckeredBackground() const;
    void DrawMainMenu() const;
    void DrawInGame() const;
    void DrawGameOver() const;
    void DrawGameObjects() const;
    void DrawGameUI() const;
    void DrawOptionsMenu() const;
    void DrawItemSlot() const;
    void DrawNameEntryScreen() const;
    void DrawHighscoreScreen() const;
    void DrawCreditsScreen() const;  // Credits method added
};

#endif