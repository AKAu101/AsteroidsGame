#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "raylib.h"
#include <string>

enum GameState {
    MAIN_MENU,           // Main menu screen with game options
    IN_GAME,             // Active gameplay state
    OPTIONS,             // Controls and settings screen
    GAME_OVER,           // Game over screen with final score
    HIGHSCORE_ENTRY,     // Name entry screen for new high scores
    HIGHSCORE_DISPLAY,   // High score table display
    CREDITS              // Credits screen showing development information
};

class GameStateManager {
private:
    GameState currentState;          // Currently active game state
    GameState previousState;         // Previously active state for reference
    bool stateChanged;               // Flag indicating recent state transition
    float stateTransitionTime;       // Time elapsed since last state change
    int menuSelection;               // Currently selected menu option index

    // Name Entry State Data
    std::string playerName;          // Player's entered name for high score
    int nameEntryScore;              // Score achieved for name entry
    int nameEntryPosition;           // Position in high score table (0-based)
    float animationTimer;            // Timer for name entry animations
    float cursorBlinkTimer;          // Timer controlling cursor blink rate
    static const int MAX_NAME_LENGTH = 5; // Maximum characters allowed in player name

public:
    GameStateManager();

    // State management
    void SetState(GameState newState /* Target state to transition to */);
    GameState GetCurrentState() const { 
        return currentState;
    }
    GameState GetPreviousState() const { 
        return previousState;
    }
    bool HasStateChanged() const { 
        return stateChanged;
    }
    void Update(float deltaTime);
    void ClearStateChange() { 
        stateChanged = false;
    }
    float GetTransitionTime() const { 
        return stateTransitionTime;
    }

    // Menu specific
    int GetMenuSelection() const { 
        return menuSelection;
    }
    void SetMenuSelection(int selection) {
        menuSelection = selection;
    }

    // Name Entry specific
    void InitializeNameEntry(int score /* Player's achieved score */,
    int position /* Position in high score rankings (0-based) */);
    const std::string& GetPlayerName() const { 
        return playerName; 
    }
    int GetNameEntryScore() const { 
        return nameEntryScore; 
    }
    int GetNameEntryPosition() const { 
        return nameEntryPosition;
    }
    float GetAnimationTimer() const {
        return animationTimer;
    }
    float GetCursorBlinkTimer() const {
        return cursorBlinkTimer;
    }
    void SetPlayerName(const std::string& name) { 
        playerName = name;
    }
    bool IsNameComplete() const { 
        return playerName.length() >= 1;
    }
    void ResetNameEntry();

    // State checks
    bool IsPlayingState() const { 
        return currentState == IN_GAME; 
    }
    bool IsMenuState() const {
        return currentState == MAIN_MENU ||
            currentState == OPTIONS ||
            currentState == GAME_OVER ||
            currentState == HIGHSCORE_ENTRY ||
            currentState == HIGHSCORE_DISPLAY ||
            currentState == CREDITS;
    }

    // Utility
    static const char* GetStateName(GameState state /* Game state to get name for */);
};

// Utility functions for interface
namespace GameStateUtils {
    void Initialize();
    void SetState(GameState newState /* Target state to transition to */);
    GameState GetCurrentState();
    GameState GetPreviousState();
    bool HasStateChanged();
    void Update(float deltaTime);
    void ClearStateChange();
    const char* GetStateName(GameState state /* Game state to get name for */);
    bool IsPlayingState();
    bool IsMenuState();
}

#endif