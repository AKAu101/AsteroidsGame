#include "gamestate.h"
#include "raylib.h"

class GameStateManager {
private:
    GameState currentState;
    GameState previousState;
    bool stateChanged;
    float stateTransitionTime;

public:
    GameStateManager();
    void SetState(GameState newState);
    GameState GetCurrentState() const { return currentState; }
    GameState GetPreviousState() const { return previousState; }
    bool HasStateChanged() const { return stateChanged; }
    void Update(float deltaTime);
    void ClearStateChange() { stateChanged = false; }
    float GetTransitionTime() const { return stateTransitionTime; }
};

GameStateManager::GameStateManager() {
    currentState = MAIN_MENU;
    previousState = MAIN_MENU;
    stateChanged = false;
    stateTransitionTime = 0.0f;
}

void GameStateManager::SetState(GameState newState) {
    if (newState != currentState) {
        previousState = currentState;
        currentState = newState;
        stateChanged = true;
        stateTransitionTime = 0.0f;
    }
}

void GameStateManager::Update(float deltaTime) {
    if (stateChanged) {
        stateTransitionTime += deltaTime;

        // After a short transition time, clear the state change flag
        if (stateTransitionTime > 0.1f) {
            stateChanged = false;
        }
    }
}

// Global state manager instance
static GameStateManager g_stateManager;

// C-style interface functions for easier use
void InitializeGameState() {
    g_stateManager = GameStateManager();
}

void SetGameState(GameState newState) {
    g_stateManager.SetState(newState);
}

GameState GetCurrentGameState() {
    return g_stateManager.GetCurrentState();
}

GameState GetPreviousGameState() {
    return g_stateManager.GetPreviousState();
}

bool HasGameStateChanged() {
    return g_stateManager.HasStateChanged();
}

void UpdateGameState(float deltaTime) {
    g_stateManager.Update(deltaTime);
}

void ClearGameStateChange() {
    g_stateManager.ClearStateChange();
}

const char* GetGameStateName(GameState state) {
    switch (state) {
    case MAIN_MENU: return "Main Menu";
    case IN_GAME: return "In Game";
    case OPTIONS: return "Options";
    case GAME_OVER: return "Game Over";
    default: return "Unknown";
    }
}

bool IsGameStatePlaying() {
    return GetCurrentGameState() == IN_GAME;
}

bool IsGameStateMenu() {
    GameState state = GetCurrentGameState();
    return state == MAIN_MENU || state == OPTIONS || state == GAME_OVER;
}