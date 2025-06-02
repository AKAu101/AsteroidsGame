#include "collisionSystem.h"
#include "globals.h"

/**
 * Constructor for CollisionSystem class
 * Initializes references to object manager and scoring system
 * @param objMgr Reference to the object manager for accessing game objects
 * @param score Reference to the scoring system for awarding points
 */
CollisionSystem::CollisionSystem(ObjectManager& objMgr, Score& score) :
    objectManager(objMgr),
    gameScore(score) {
}

/**
 * Main collision detection method called each frame
 * Coordinates all collision checks between different object types
 * Handles shield protection and invulnerability states
 * @param hasShield Whether the player currently has shield protection
 * @param isInvulnerable Whether the player is currently invulnerable
 */
void CollisionSystem::CheckCollisions(bool hasShield, bool isInvulnerable) {
    CheckProjectileAsteroidCollisions();
    CheckPlayerAsteroidCollisions(hasShield, isInvulnerable);
}

/**
 * Checks collisions between all active projectiles and asteroids
 * Handles asteroid destruction, fragmentation into smaller pieces
 * Awards points based on asteroid size and manages power-up spawning
 * Prevents multiple collisions per frame for stability
 */
void CollisionSystem::CheckProjectileAsteroidCollisions() {
    auto& projectiles = const_cast<std::vector<Projectile>&>(objectManager.GetProjectiles());
    auto& asteroids = const_cast<std::vector<Asteroid>&>(objectManager.GetAsteroids());

    bool collisionFound = false;

    for (auto& projectile : projectiles) {
        if (!projectile.IsActive() || collisionFound) continue;

        for (auto& asteroid : asteroids) {
            if (!asteroid.IsActive()) continue;

            if (CheckCollisionRecs(projectile.GetBounds(), asteroid.GetBounds())) {
                // IMMEDIATELY deactivate both objects to prevent multiple collisions
                projectile.Deactivate();

                // Save data before deactivation
                Vector2 asteroidPos = asteroid.GetPosition();
                AsteroidSize currentSize = asteroid.GetSize();
                int points = asteroid.GetPoints();

                // IMMEDIATELY deactivate asteroid
                asteroid.Destroy();

                // Add points
                gameScore.AddPoints(points);

                // Spawn smaller asteroids - but only once!
                if (currentSize == LARGE) {
                    // Slightly offset positions to avoid overlap
                    Vector2 pos1 = { asteroidPos.x + 20, asteroidPos.y + 20 };
                    Vector2 pos2 = { asteroidPos.x - 20, asteroidPos.y - 20 };
                    objectManager.SpawnAsteroid(pos1, MEDIUM);
                    objectManager.SpawnAsteroid(pos2, MEDIUM);
                }
                else if (currentSize == MEDIUM) {
                    // Slightly offset positions to avoid overlap
                    Vector2 pos1 = { asteroidPos.x + 15, asteroidPos.y + 15 };
                    Vector2 pos2 = { asteroidPos.x - 15, asteroidPos.y - 15 };
                    objectManager.SpawnAsteroid(pos1, SMALL);
                    objectManager.SpawnAsteroid(pos2, SMALL);
                }
                // SMALL asteroids spawn nothing

                // Power-up spawn chance (only for large asteroids)
                if (currentSize == LARGE && GetRandomValue(0, 100) < 20) { // 20% chance
                    PowerUpType randomType = static_cast<PowerUpType>(GetRandomValue(0, 2));
                    objectManager.SpawnPowerUp(asteroidPos, randomType);
                }

                collisionFound = true;
                break;
            }
        }

        if (collisionFound) break;
    }
}

/**
 * Checks collisions between the player spaceship and all asteroids
 * Respects invulnerability status and handles shield protection
 * Manages life loss, shield destruction, and collision consequences
 * @param hasShield Whether the player currently has shield protection
 * @param isInvulnerable Whether the player is currently invulnerable
 */
void CollisionSystem::CheckPlayerAsteroidCollisions(bool hasShield, bool isInvulnerable) {
    if (isInvulnerable) return; // Player is invulnerable - no collisions

    Spaceship& player = objectManager.GetPlayer();
    auto& asteroids = const_cast<std::vector<Asteroid>&>(objectManager.GetAsteroids());

    Rectangle playerBounds = player.GetBounds();

    for (auto& asteroid : asteroids) {
        if (!asteroid.IsActive()) continue;

        if (CheckCollisionRecs(playerBounds, asteroid.GetBounds())) {
            // IMMEDIATELY deactivate asteroid
            asteroid.Destroy();

            // Check if shield is active
            if (player.IsShieldActive()) {
                // Shield absorbs damage and gets destroyed
                player.DeactivateShield();
                return; // Player doesn't lose a life
            }

            // Player loses a life (only if no shield and not invulnerable)
            player.LoseLife();
            break; // Only one collision per frame
        }
    }
}