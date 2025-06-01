#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "gamestate.h"
#include "score.h"

class UIRenderer {
private:
    GameStateManager& stateManager;
    Score& gameScore;

public:
    UIRenderer(GameStateManager& stateMgr, Score& score);

    void DrawCurrentState() const;

private:
    void DrawCheckeredBackground() const;
    void DrawMainMenu() const;
    void DrawInGame() const;
    void DrawGameOver() const;
};

#endif