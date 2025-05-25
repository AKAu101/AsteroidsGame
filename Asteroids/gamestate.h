#ifndef GAMESTATE_H
#define GAMESTATE_H

enum GameState {
    MAIN_MENU,
    IN_GAME,
    OPTIONS,
    GAME_OVER
};

// Game State Management Functions
void InitializeGameState();
void SetGameState(GameState newState);
GameState GetCurrentGameState();
GameState GetPreviousGameState();
bool HasGameStateChanged();
void UpdateGameState(float deltaTime);
void ClearGameStateChange();

// Utility Functions
const char* GetGameStateName(GameState state);
bool IsGameStatePlaying();
bool IsGameStateMenu();

#endif