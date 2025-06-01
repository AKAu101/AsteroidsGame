#include "gamestate.h"
#include <iostream>

// Global instance (if needed for C-style interface)
static GameStateManager g_stateManager;

// GameStateManager implementation
GameStateManager::GameStateManager() :
    currentState(MAIN_MENU),
    previousState(MAIN_MENU),
    stateChanged(false),
    stateTransitionTime(0.0f),
    menuSelection(0),
    playerName(""),
    nameEntryScore(0),
    nameEntryPosition(0),
    animationTimer(0.0f),
    cursorBlinkTimer(0.0f) {
}

void GameStateManager::SetState(GameState newState) {
    if (newState != currentState) {
        previousState = currentState;
        currentState = newState;
        stateChanged = true;
        stateTransitionTime = 0.0f;

        // Reset menu selection when returning to menu
        if (newState == MAIN_MENU) {
            menuSelection = 0;
        }
    }
}

void GameStateManager::Update(float deltaTime) {
    if (stateChanged) {
        stateTransitionTime += deltaTime;

        if (stateTransitionTime > 0.1f) {
            stateChanged = false;
        }
    }

    // Update animation timers for name entry
    if (currentState == HIGHSCORE_ENTRY) {
        animationTimer += deltaTime;
        cursorBlinkTimer += deltaTime;
    }
}

void GameStateManager::InitializeNameEntry(int score, int position) {
    nameEntryScore = score;
    nameEntryPosition = position;
    playerName = "";
    animationTimer = 0.0f;
    cursorBlinkTimer = 0.0f;
}

void GameStateManager::ResetNameEntry() {
    playerName = "";
    nameEntryScore = 0;
    nameEntryPosition = 0;
    animationTimer = 0.0f;
    cursorBlinkTimer = 0.0f;
}

const char* GameStateManager::GetStateName(GameState state) {
    switch (state) {
    case MAIN_MENU: return "Main Menu";
    case IN_GAME: return "In Game";
    case OPTIONS: return "Options";
    case GAME_OVER: return "Game Over";
    case HIGHSCORE_ENTRY: return "Highscore Entry";
    case HIGHSCORE_DISPLAY: return "Highscore Display";
    case CREDITS: return "Credits";
    default: return "Unknown";
    }
}

// C-style interface implementation
namespace GameStateUtils {
    void Initialize() {
        g_stateManager = GameStateManager();
    }

    void SetState(GameState newState) {
        g_stateManager.SetState(newState);
    }

    GameState GetCurrentState() {
        return g_stateManager.GetCurrentState();
    }

    GameState GetPreviousState() {
        return g_stateManager.GetPreviousState();
    }

    bool HasStateChanged() {
        return g_stateManager.HasStateChanged();
    }

    void Update(float deltaTime) {
        g_stateManager.Update(deltaTime);
    }

    void ClearStateChange() {
        g_stateManager.ClearStateChange();
    }

    const char* GetStateName(GameState state) {
        return GameStateManager::GetStateName(state);
    }

    bool IsPlayingState() {
        return g_stateManager.IsPlayingState();
    }

    bool IsMenuState() {
        return g_stateManager.IsMenuState();
    }
}