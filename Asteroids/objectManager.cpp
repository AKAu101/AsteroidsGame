// objectmanager.cpp - With PowerUps
#include "objectmanager.h"
#include "globals.h"
#include <algorithm>
#include <cstdlib>

/**
 * Constructor for ObjectManager class
 * Initializes the object manager with a reference to the player spaceship
 * Pre-allocates memory for game objects to improve performance
 * @param ship Reference to the player's spaceship object
 */
ObjectManager::ObjectManager(Spaceship& ship) : player(ship), powerupSpawnTimer(0.0f) {
    projectiles.reserve(50);
    asteroids.reserve(20);
    powerups.reserve(10);
}

/**
 * Updates all game objects and manages their lifecycle
 * Updates projectiles, asteroids, and power-ups each frame
 * Handles power-up spawning logic and removes inactive objects
 * Uses erase-remove idiom for efficient object cleanup
 * @param deltaTime Time elapsed since last frame in seconds
 */
void ObjectManager::UpdateObjects(float deltaTime) {
    // Update projectiles
    for (auto& projectile : projectiles) {
        projectile.Update(deltaTime);
    }

    // Update asteroids
    for (auto& asteroid : asteroids) {
        asteroid.Update(deltaTime);
    }

    // Update power-ups
    for (auto& powerup : powerups) {
        powerup.Update(deltaTime);
    }

    // Check power-up spawning
    CheckPowerUpSpawning(deltaTime);

    // Remove inactive objects
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

/**
 * Manages automatic power-up spawning based on timer intervals
 * Spawns power-ups at random screen positions (avoiding edges)
 * Selects random power-up types and resets spawn timer
 * @param deltaTime Time elapsed since last frame in seconds
 */
void ObjectManager::CheckPowerUpSpawning(float deltaTime) {
    powerupSpawnTimer += deltaTime;

    if (powerupSpawnTimer >= POWERUP_SPAWN_INTERVAL) {
        // Spawn at random position on screen (not at edges)
        Vector2 spawnPos = {
            static_cast<float>(GetRandomValue(100, SCREEN_WIDTH - 100)),
            static_cast<float>(GetRandomValue(100, SCREEN_HEIGHT - 100))
        };

        // Random power-up type
        PowerUpType randomType = static_cast<PowerUpType>(GetRandomValue(0, 2));
        SpawnPowerUp(spawnPos, randomType);
        powerupSpawnTimer = 0.0f;
    }
}

/**
 * Creates and spawns a new power-up at the specified location
 * Adds the power-up to the active power-ups collection
 * @param position 2D vector position where the power-up should spawn
 * @param type Type of power-up to create (EXTRA_LIFE, RAPID_FIRE, SHIELD)
 */
void ObjectManager::SpawnPowerUp(Vector2 position, PowerUpType type) {
    PowerUp newPowerUp;
    newPowerUp.Spawn(position, type);
    powerups.push_back(newPowerUp);
}

/**
 * Creates and fires a projectile from the specified position and rotation
 * Reuses inactive projectiles for performance, creates new ones if needed
 * Enforces maximum projectile limit to prevent memory issues
 * @param position Starting position for the projectile
 * @param rotation Direction angle for the projectile's movement
 */
void ObjectManager::FireProjectile(Vector2 position, float rotation) {
    // Find free projectile or add new one
    for (auto& projectile : projectiles) {
        if (!projectile.IsActive()) {
            projectile.Fire(position, rotation);
            return;
        }
    }

    // If no free projectile found, add new one
    if (projectiles.size() < 50) { // Limit
        Projectile newProjectile;
        newProjectile.Fire(position, rotation);
        projectiles.push_back(newProjectile);
    }
}

/**
 * Spawns multiple large asteroids at random edge positions
 * Used for level initialization and wave progression
 * @param count Number of asteroids to spawn
 */
void ObjectManager::SpawnAsteroids(int count) {
    for (int i = 0; i < count; i++) {
        SpawnAsteroid(GetRandomEdgePosition(), LARGE);
    }
}

/**
 * Creates and spawns a single asteroid at the specified position and size
 * Adds the asteroid to the active asteroids collection
 * @param position 2D vector position where the asteroid should spawn
 * @param size Size category of the asteroid (LARGE, MEDIUM, SMALL)
 */
void ObjectManager::SpawnAsteroid(Vector2 position, AsteroidSize size) {
    Asteroid newAsteroid;
    newAsteroid.Spawn(position, size);
    asteroids.push_back(newAsteroid);
}

/**
 * Generates a random position along the screen edges for asteroid spawning
 * Positions asteroids slightly outside screen boundaries for smooth entry
 * Randomly selects from top, right, bottom, or left edges
 * @return Vector2 position coordinates for spawning
 */
Vector2 ObjectManager::GetRandomEdgePosition() {
    int edge = GetRandomValue(0, 3); // 0=top, 1=right, 2=bottom, 3=left
    Vector2 position;

    switch (edge) {
    case 0: // Top
        position.x = static_cast<float>(GetRandomValue(0, SCREEN_WIDTH));
        position.y = -50.0f;
        break;
    case 1: // Right
        position.x = SCREEN_WIDTH + 50.0f;
        position.y = static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT));
        break;
    case 2: // Bottom
        position.x = static_cast<float>(GetRandomValue(0, SCREEN_WIDTH));
        position.y = SCREEN_HEIGHT + 50.0f;
        break;
    case 3: // Left
        position.x = -50.0f;
        position.y = static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT));
        break;
    }

    return position;
}

/**
 * Resets all game objects to initial state
 * Clears all projectiles, asteroids, and power-ups
 * Resets power-up spawn timer for new game sessions
 */
void ObjectManager::ResetObjects() {
    projectiles.clear();
    asteroids.clear();
    powerups.clear();
    powerupSpawnTimer = 0.0f;
}