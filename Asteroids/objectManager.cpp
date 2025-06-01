// objectmanager.cpp - Mit PowerUps
#include "objectmanager.h"
#include "globals.h"
#include <algorithm>
#include <cstdlib>

ObjectManager::ObjectManager(Spaceship& ship) : player(ship), powerupSpawnTimer(0.0f) {
    projectiles.reserve(50);
    asteroids.reserve(20);
    powerups.reserve(10);
}

void ObjectManager::UpdateObjects(float deltaTime) {
    // Update Projektile
    for (auto& projectile : projectiles) {
        projectile.Update(deltaTime);
    }

    // Update Asteroiden
    for (auto& asteroid : asteroids) {
        asteroid.Update(deltaTime);
    }

    // Update PowerUps
    for (auto& powerup : powerups) {
        powerup.Update(deltaTime);
    }

    // PowerUp Spawning prüfen
    CheckPowerUpSpawning(deltaTime);

    // Inaktive Objekte entfernen
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.IsActive(); }),
        projectiles.end()
    );

    powerups.erase(
        std::remove_if(powerups.begin(), powerups.end(),
            [](const PowerUp& p) { return !p.IsActive(); }),
        powerups.end()
    );
}

void ObjectManager::CheckPowerUpSpawning(float deltaTime) {
    powerupSpawnTimer += deltaTime;

    if (powerupSpawnTimer >= POWERUP_SPAWN_INTERVAL) {
        // Zufällige Position auf dem Bildschirm spawnen (nicht am Rand)
        Vector2 spawnPos = {
            static_cast<float>(GetRandomValue(100, SCREEN_WIDTH - 100)),
            static_cast<float>(GetRandomValue(100, SCREEN_HEIGHT - 100))
        };

        // Zufälliger PowerUp-Typ
        PowerUpType randomType = static_cast<PowerUpType>(GetRandomValue(0, 2));

        SpawnPowerUp(spawnPos, randomType);
        powerupSpawnTimer = 0.0f;
    }
}

void ObjectManager::SpawnPowerUp(Vector2 position, PowerUpType type) {
    PowerUp newPowerUp;
    newPowerUp.Spawn(position, type);
    powerups.push_back(newPowerUp);
}

void ObjectManager::FireProjectile(Vector2 position, float rotation) {
    // Freies Projektil finden oder neues hinzufügen
    for (auto& projectile : projectiles) {
        if (!projectile.IsActive()) {
            projectile.Fire(position, rotation);
            return;
        }
    }

    // Wenn kein freies Projektil gefunden, neues hinzufügen
    if (projectiles.size() < 50) { // Limit
        Projectile newProjectile;
        newProjectile.Fire(position, rotation);
        projectiles.push_back(newProjectile);
    }
}

void ObjectManager::SpawnAsteroids(int count) {
    for (int i = 0; i < count; i++) {
        SpawnAsteroid(GetRandomEdgePosition(), LARGE);
    }
}

void ObjectManager::SpawnAsteroid(Vector2 position, AsteroidSize size) {
    Asteroid newAsteroid;
    newAsteroid.Spawn(position, size);
    asteroids.push_back(newAsteroid);
}

Vector2 ObjectManager::GetRandomEdgePosition() {
    int edge = GetRandomValue(0, 3); // 0=oben, 1=rechts, 2=unten, 3=links
    Vector2 position;

    switch (edge) {
    case 0: // Oben
        position.x = static_cast<float>(GetRandomValue(0, SCREEN_WIDTH));
        position.y = -50.0f;
        break;
    case 1: // Rechts
        position.x = SCREEN_WIDTH + 50.0f;
        position.y = static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT));
        break;
    case 2: // Unten
        position.x = static_cast<float>(GetRandomValue(0, SCREEN_WIDTH));
        position.y = SCREEN_HEIGHT + 50.0f;
        break;
    case 3: // Links
        position.x = -50.0f;
        position.y = static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT));
        break;
    }

    return position;
}

void ObjectManager::ResetObjects() {
    projectiles.clear();
    asteroids.clear();
    powerups.clear();
    powerupSpawnTimer = 0.0f;
}