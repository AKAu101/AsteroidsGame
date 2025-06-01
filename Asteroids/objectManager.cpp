#include "objectManager.h"
#include "globals.h"
#include <ctime>

ObjectManager::ObjectManager(Spaceship& player) : player(player) {
    asteroids.resize(MAX_ASTEROIDS);
    projectiles.resize(MAX_PROJECTILES);
    powerups.resize(MAX_POWERUPS);
    srand(static_cast<unsigned int>(time(nullptr)));
}

void ObjectManager::SpawnAsteroids(int count) {
    for (int i = 0; i < count; i++) {
        SpawnAsteroid(GetRandomEdgePosition(), LARGE);
    }
}

void ObjectManager::SpawnAsteroid(Vector2 position, AsteroidSize size) {
    for (auto& asteroid : asteroids) {
        if (!asteroid.IsActive()) {
            asteroid.Spawn(position, size);
            return;
        }
    }
}

void ObjectManager::SpawnPowerUp(Vector2 position) {
    for (auto& powerup : powerups) {
        if (!powerup.IsActive()) {
            PowerUpType type = static_cast<PowerUpType>(rand() % 3);
            powerup.Spawn(position, type);
            return;
        }
    }
}

void ObjectManager::FireProjectile(Vector2 position, float rotation) {
    for (auto& projectile : projectiles) {
        if (!projectile.IsActive()) {
            projectile.Fire(position, rotation);
            return;
        }
    }
}

void ObjectManager::UpdateObjects(float deltaTime) {
    for (auto& asteroid : asteroids) {
        asteroid.Update(deltaTime);
    }
    for (auto& projectile : projectiles) {
        projectile.Update(deltaTime);
    }
    for (auto& powerup : powerups) {
        powerup.Update(deltaTime);
    }
}

void ObjectManager::ResetObjects() {
    for (auto& asteroid : asteroids) {
        asteroid.Destroy();
    }
    for (auto& projectile : projectiles) {
        projectile.Deactivate();
    }
    for (auto& powerup : powerups) {
        powerup.Collect();
    }
}

Vector2 ObjectManager::GetRandomEdgePosition() const {
    Vector2 pos = { 0, 0 };
    int side = rand() % 4;

    switch (side) {
    case 0: pos = { static_cast<float>(rand() % SCREEN_WIDTH), -50.0f }; break;
    case 1: pos = { SCREEN_WIDTH + 50.0f, static_cast<float>(rand() % SCREEN_HEIGHT) }; break;
    case 2: pos = { static_cast<float>(rand() % SCREEN_WIDTH), SCREEN_HEIGHT + 50.0f }; break;
    case 3: pos = { -50.0f, static_cast<float>(rand() % SCREEN_HEIGHT) }; break;
    }
    return pos;
}