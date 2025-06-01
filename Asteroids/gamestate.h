#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "raylib.h"

enum GameState {
    MAIN_MENU,
    IN_GAME,
    OPTIONS,
    GAME_OVER
};

class GameStateManager {
private:
    GameState currentState;
    GameState previousState;
    bool stateChanged;
    float stateTransitionTime;
    int menuSelection;

public:
    GameStateManager();

    // State management
    void SetState(GameState newState);
    GameState GetCurrentState() const { return currentState; }
    GameState GetPreviousState() const { return previousState; }
    bool HasStateChanged() const { return stateChanged; }
    void Update(float deltaTime);
    void ClearStateChange() { stateChanged = false; }
    float GetTransitionTime() const { return stateTransitionTime; }

    // Menu specific
    int GetMenuSelection() const { return menuSelection; }
    void SetMenuSelection(int selection) { menuSelection = selection; }

    // State checks
    bool IsPlayingState() const { return currentState == IN_GAME; }
    bool IsMenuState() const {
        return currentState == MAIN_MENU ||
            currentState == OPTIONS ||
            currentState == GAME_OVER;
    }

    // Utility
    static const char* GetStateName(GameState state);
};

// Utility functions for C-style interface (if needed)
namespace GameStateUtils {
    void Initialize();
    void SetState(GameState newState);
    GameState GetCurrentState();
    GameState GetPreviousState();
    bool HasStateChanged();
    void Update(float deltaTime);
    void ClearStateChange();
    const char* GetStateName(GameState state);
    bool IsPlayingState();
    bool IsMenuState();
}

#endif