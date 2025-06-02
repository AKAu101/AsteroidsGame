#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "gamestate.h"
#include "objectmanager.h"
#include "collisionsystem.h"
#include "inputhandler.h"
#include "uirenderer.h"
#include "score.h"
#include "highscoreManager.h"
#include "globals.h"

class Game {
private:
    float projectileCooldown;     // Time remaining before next projectile can be fired
    float asteroidSpawnTimer;     // Timer for automatic asteroid spawning intervals
    int currentItem;              // Currently held power-up item (0=none, 1=rapid, 2=shield, 3=life)
    int amountRapid;              // Remaining rapid fire shots when rapid fire is active
    bool hasRapid;                // Whether rapid fire mode is currently active
    bool hasShield;               // Whether shield protection is currently active
    Sound shootSound;             // Sound effect played when firing projectiles
    Music backgroundMusic;        // Background music stream for atmospheric audio
    bool gameRunning = true;      // Master flag controlling main game loop execution

    // Game objects
    Spaceship player;             // Player-controlled spaceship with movement and combat
    Score gameScore;              // Scoring system managing points, levels, and high scores

    // Subsystems
    GameStateManager stateManager;   // Manages current game state and transitions
    ObjectManager objectManager;     // Handles all game objects (asteroids, projectiles, power-ups)
    CollisionSystem collisionSystem; // Detects and handles all collision interactions
    InputHandler inputHandler;       // Processes user input for different game states
    UIRenderer uiRenderer;           // Renders all user interface and visual elements
    HighscoreManager highscoreManager; // Manages persistent high score storage and display

public:
    Game();
    ~Game();
    void Initialize();
    void Update();
    void Draw();
    bool ShouldClose() {
        if (gameRunning == false) {
            return true;
        }
        else {
            return false;
        }
    }

    bool IsGameRunning() const { return gameRunning; }
    void SetGameRunning(bool running) {
        gameRunning = running;
    }
    void PlayShootSound();
    void AddHighscoreEntry(const std::string& name /* Player's name for the high score entry (max 5 characters) */,
        int score /* Achieved score value for the entry */);
    HighscoreManager& GetHighscoreManager(){ 
        return highscoreManager; 
    }
    void ResetGame();

private:
    void InitGameSounds();
    void UnloadGameSounds();
    void UpdateTimers(float deltaTime);
    void HandleSpawning(float deltaTime);
    void CheckGameState();
    void UpdateInGame(float deltaTime);
    void CheckPowerUpCollisions();
    void HandleGameOver();
};

#endif