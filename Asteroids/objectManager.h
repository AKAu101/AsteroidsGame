// objectmanager.h - Erweitert um PowerUps
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include "spaceship.h"
#include "projectile.h"
#include "asteroid.h"
#include "powerup.h"

class ObjectManager {
private:
    Spaceship& player;
    std::vector<Projectile> projectiles;
    std::vector<Asteroid> asteroids;
    std::vector<PowerUp> powerups;

    float powerupSpawnTimer;
    const float POWERUP_SPAWN_INTERVAL = 15.0f; // Alle 15 Sekunden

public:
    ObjectManager(Spaceship& ship);

    void UpdateObjects(float deltaTime);
    void ResetObjects();

    // Spawning
    void SpawnAsteroids(int count);
    void SpawnAsteroid(Vector2 position, AsteroidSize size);
    void FireProjectile(Vector2 position, float rotation);
    void SpawnPowerUp(Vector2 position, PowerUpType type);
    void CheckPowerUpSpawning(float deltaTime);
    Vector2 GetRandomEdgePosition();

    // Getters
    Spaceship& GetPlayer() { return player; }
    const std::vector<Projectile>& GetProjectiles() const { return projectiles; }
    const std::vector<Asteroid>& GetAsteroids() const { return asteroids; }
    const std::vector<PowerUp>& GetPowerUps() const { return powerups; }

    // Mutable Getters (falls benötigt)
    std::vector<Projectile>& GetProjectilesMutable() { return projectiles; }
    std::vector<Asteroid>& GetAsteroidsMutable() { return asteroids; }
    std::vector<PowerUp>& GetPowerUpsMutable() { return powerups; }
};

#endif