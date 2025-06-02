// objectmanager.h - Extended with PowerUps
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include "spaceship.h"
#include "projectile.h"
#include "asteroid.h"
#include "powerup.h"

class ObjectManager {
private:
    Spaceship& player;                         // Reference to player spaceship
    std::vector<Projectile> projectiles;       // Collection of all active projectiles
    std::vector<Asteroid> asteroids;           // Collection of all active asteroids
    std::vector<PowerUp> powerups;             // Collection of all active power-ups
    float powerupSpawnTimer;                   // Timer for automatic power-up spawning
    const float POWERUP_SPAWN_INTERVAL = 15.0f; // Spawn interval in seconds (every 15 seconds)

public:
    ObjectManager(Spaceship& ship /* Reference to player spaceship */);

    void UpdateObjects(float deltaTime);
    void ResetObjects();

    // Spawning
    void SpawnAsteroids(int count /* Number of asteroids to spawn */);
    void SpawnAsteroid(Vector2 position /* Spawn position coordinates */,
        AsteroidSize size /* Size category (LARGE, MEDIUM, SMALL) */);
    void FireProjectile(Vector2 position /* Starting position for projectile */,
        float rotation /* Direction angle in degrees */);
    void SpawnPowerUp(Vector2 position /* Spawn position coordinates */,
        PowerUpType type /* Type of power-up to create */);
    void CheckPowerUpSpawning(float deltaTime /* Time elapsed since last frame in seconds */);
    Vector2 GetRandomEdgePosition();

    // Getters
    Spaceship& GetPlayer() { return player; }
    const std::vector<Projectile>& GetProjectiles() const { 
        return projectiles;
    }
    const std::vector<Asteroid>& GetAsteroids() const {
        return asteroids;
    }
    const std::vector<PowerUp>& GetPowerUps() const {
        return powerups;
    }

    // Mutable Getters (if needed)
    std::vector<Projectile>& GetProjectilesMutable() {
        return projectiles;
    }
    std::vector<Asteroid>& GetAsteroidsMutable() {
        return asteroids; 
    }
    std::vector<PowerUp>& GetPowerUpsMutable() { 
        return powerups; 
    }
};

#endif  