// game.h - Header für Game-Klasse
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "spaceship.h"
#include "asteroid.h"
#include "projectile.h"
#include "powerup.h"
#include "score.h"
#include "gamestate.h"
#include <vector>

class Game {
private:
    GameState currentState;
    Spaceship player;
    std::vector<Asteroid> asteroids;
    std::vector<Projectile> projectiles;
    std::vector<PowerUp> powerups;
    Score gameScore;
    float projectileCooldown;
    float asteroidSpawnTimer;
    int menuSelection;
    bool gameRunning;

public:
    Game();
    ~Game();
    void Initialize();
    void Update();
    void Draw();
    bool ShouldClose() const { return !gameRunning; }

private:
    // PHASE 1: Input Handling (zentralisiert)
    void HandleInput(float deltaTime);
    void HandleGameInput(float deltaTime);
    void FireProjectile();

    // PHASE 2: State Updates (kein Input)
    void UpdateMainMenu();
    void UpdateInGame(float deltaTime);
    void UpdateGameOver();
    void UpdateTimers(float deltaTime);

    // PHASE 3: Game Logic
    void HandleSpawning(float deltaTime);
    void CheckGameState();
    void CheckCollisions();
    void HandleAsteroidDestruction(int asteroidIndex);

    // PHASE 4: Drawing (const methods)
    void DrawCheckeredBackground();  // Neue Methode für karierten Hintergrund
    void DrawMainMenu();
    void DrawInGame();
    void DrawGameOver();

    // Utility methods
    void SpawnAsteroids(int count);
    void SpawnAsteroid(Vector2 position, AsteroidSize size);
    void SpawnPowerUp(Vector2 position);
    bool CheckCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2);
    void ResetGame();
    Vector2 GetRandomEdgePosition();
};

#endif