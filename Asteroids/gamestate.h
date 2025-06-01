#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "raylib.h"
#include <string>

enum GameState {
    MAIN_MENU,
    IN_GAME,
    OPTIONS,
    GAME_OVER,
    HIGHSCORE_ENTRY,
    HIGHSCORE_DISPLAY,
    CREDITS  // Neuer State für Credits
};

class GameStateManager {
private:
    GameState currentState;
    GameState previousState;
    bool stateChanged;
    float stateTransitionTime;
    int menuSelection;

    // Name Entry State Data
    std::string playerName;
    int nameEntryScore;
    int nameEntryPosition;
    float animationTimer;
    float cursorBlinkTimer;
    static const int MAX_NAME_LENGTH = 5;

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

    // Name Entry specific
    void InitializeNameEntry(int score, int position);
    const std::string& GetPlayerName() const { return playerName; }
    int GetNameEntryScore() const { return nameEntryScore; }
    int GetNameEntryPosition() const { return nameEntryPosition; }
    float GetAnimationTimer() const { return animationTimer; }
    float GetCursorBlinkTimer() const { return cursorBlinkTimer; }
    void SetPlayerName(const std::string& name) { playerName = name; }
    bool IsNameComplete() const { return playerName.length() >= 1; }
    void ResetNameEntry();

    // State checks
    bool IsPlayingState() const { return currentState == IN_GAME; }
    bool IsMenuState() const {
        return currentState == MAIN_MENU ||
            currentState == OPTIONS ||
            currentState == GAME_OVER ||
            currentState == HIGHSCORE_ENTRY ||
            currentState == HIGHSCORE_DISPLAY ||
            currentState == CREDITS;
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