#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "asteroid.h"
#include "projectile.h"
#include "powerup.h"
#include "spaceship.h"
#include <vector>

class ObjectManager {
private:
    std::vector<Asteroid> asteroids;
    std::vector<Projectile> projectiles;
    std::vector<PowerUp> powerups;
    Spaceship& player;

public:
    ObjectManager(Spaceship& player);

    void SpawnAsteroids(int count);
    void SpawnAsteroid(Vector2 position, AsteroidSize size);
    void SpawnPowerUp(Vector2 position);
    void FireProjectile(Vector2 position, float rotation);

    void UpdateObjects(float deltaTime);
    void ResetObjects();

    Vector2 GetRandomEdgePosition() const;

    // Single declaration of each getter function
    Spaceship& GetPlayer() { return player; }
    const std::vector<Asteroid>& GetAsteroids() const { return asteroids; }
    std::vector<Asteroid>& GetAsteroids() { return asteroids; }
    const std::vector<Projectile>& GetProjectiles() const { return projectiles; }
    std::vector<Projectile>& GetProjectiles() { return projectiles; }
    const std::vector<PowerUp>& GetPowerups() const { return powerups; }
    std::vector<PowerUp>& GetPowerups() { return powerups; }
};

#endif