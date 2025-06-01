#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "gamestate.h"
#include "objectmanager.h"
#include "collisionsystem.h"
#include "inputhandler.h"
#include "uirenderer.h"
#include "score.h"
#include "globals.h"

class Game {
private:
    float projectileCooldown;
    float asteroidSpawnTimer;
    int currentItem;
    int amountRapid;
    bool hasRapid;
    bool hasShield;
    Sound shootSound;
    Music backgroundMusic; // Musik-Variable hinzugefügt
    bool gameRunning = true;

    // Game objects
    Spaceship player;
    Score gameScore;

    // Subsystems
    GameStateManager stateManager;
    ObjectManager objectManager;
    CollisionSystem collisionSystem;
    InputHandler inputHandler;
    UIRenderer uiRenderer;

public:
    Game();
    ~Game();
    void Initialize();
    void Update();
    void Draw();
    bool ShouldClose()
    {
        if (gameRunning == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool IsGameRunning() const { return gameRunning; }
    void SetGameRunning(bool running) { gameRunning = running; }
    void PlayShootSound(); // Schuss-Sound Methode

private:
    void InitGameSounds();
    void UnloadGameSounds();
    void UpdateTimers(float deltaTime);
    void HandleSpawning(float deltaTime);
    void CheckGameState();
    void UpdateInGame(float deltaTime);
    void ResetGame();
    void CheckPowerUpCollisions();
};

#endif