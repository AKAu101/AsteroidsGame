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
    int& currentItem;
    class HighscoreManager* highscoreManager = nullptr;

public:
    UIRenderer(GameStateManager& stateMgr, Score& score, Spaceship& ship, ObjectManager& objMgr, int& currentItemRef);

    void DrawCurrentState() const;
    void SetHighscoreManager(class HighscoreManager* hsMgr) { highscoreManager = hsMgr; }

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
    void DrawCreditsScreen() const;  // Credits-Methode hinzugefügt
};

#endif