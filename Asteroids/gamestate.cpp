#include "gamestate.h"
#include <iostream>

// Global instance for C-style interface compatibility
static GameStateManager g_stateManager;

/**
 * Constructor for GameStateManager class
 * Initializes all state management variables to default values
 * Sets initial state to main menu and resets all timers and counters
 */
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

/**
 * Changes the current game state to a new state
 * Updates previous state reference and triggers state change flag
 * Resets menu selection when returning to main menu
 * @param newState The target game state to transition to
 */
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

/**
 * Updates state manager timers and handles state transition logic
 * Manages state change cooldown period and updates animation timers
 * Specifically handles name entry state animation and cursor blinking
 * @param deltaTime Time elapsed since last frame in seconds
 */
void GameStateManager::Update(float deltaTime) {
    if (stateChanged) {
        stateTransitionTime += deltaTime;
        if (stateTransitionTime > 0.1f) {
            stateChanged = false;
        }
    }

    // Update animation timers for name entry state
    if (currentState == HIGHSCORE_ENTRY) {
        animationTimer += deltaTime;
        cursorBlinkTimer += deltaTime;
    }
}

/**
 * Initializes the name entry system for highscore recording
 * Sets up score, position, and resets name input and timers
 * Called when player achieves a new highscore
 * @param score The player's achieved score
 * @param position The position in the highscore table (1st, 2nd, etc.)
 */
void GameStateManager::InitializeNameEntry(int score, int position) {
    nameEntryScore = score;
    nameEntryPosition = position;
    playerName = "";
    animationTimer = 0.0f;
    cursorBlinkTimer = 0.0f;
}

/**
 * Resets all name entry related data to default values
 * Clears player name, score, position, and animation timers
 * Used when exiting name entry or starting a new game
 */
void GameStateManager::ResetNameEntry() {
    playerName = "";
    nameEntryScore = 0;
    nameEntryPosition = 0;
    animationTimer = 0.0f;
    cursorBlinkTimer = 0.0f;
}

/**
 * Returns a human-readable string representation of a game state
 * @param state The game state to get the name for
 * @return Const character pointer to the state name string
 */
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

// C-style interface implementation for compatibility with C code
namespace GameStateUtils {

    /**
     * Initializes the global game state manager instance
     * Creates a fresh GameStateManager with default settings
     */
    void Initialize() {
        g_stateManager = GameStateManager();
    }

    /**
     * Sets the current game state through the global instance
     * @param newState The target game state to transition to
     */
    void SetState(GameState newState) {
        g_stateManager.SetState(newState);
    }

    /**
     * Gets the current active game state
     * @return Current GameState enum value
     */
    GameState GetCurrentState() {
        return g_stateManager.GetCurrentState();
    }

    /**
     * Gets the previously active game state
     * @return Previous GameState enum value
     */
    GameState GetPreviousState() {
        return g_stateManager.GetPreviousState();
    }

    /**
     * Checks if a state change has occurred recently
     * @return True if state has changed and is still in transition period
     */
    bool HasStateChanged() {
        return g_stateManager.HasStateChanged();
    }

    /**
     * Updates the global state manager with delta time
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void Update(float deltaTime) {
        g_stateManager.Update(deltaTime);
    }

    /**
     * Manually clears the state change flag
     * Forces the state manager to consider the transition complete
     */
    void ClearStateChange() {
        g_stateManager.ClearStateChange();
    }

    /**
     * Gets the human-readable name of a game state
     * @param state The game state to get the name for
     * @return Const character pointer to the state name string
     */
    const char* GetStateName(GameState state) {
        return GameStateManager::GetStateName(state);
    }

    /**
     * Checks if the current state is a gameplay state
     * @return True if currently in an active playing state
     */
    bool IsPlayingState() {
        return g_stateManager.IsPlayingState();
    }

    /**
     * Checks if the current state is a menu-based state
     * @return True if currently in a menu or UI state
     */
    bool IsMenuState() {
        return g_stateManager.IsMenuState();
    }
}