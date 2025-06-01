#include "collisionsystem.h"
#include "globals.h"

CollisionSystem::CollisionSystem(ObjectManager& objManager, Score& score) :
    objectManager(objManager),
    gameScore(score) {
}

void CollisionSystem::CheckCollisions(bool& hasShield, bool isInvulnerable) {
    Spaceship& player = objectManager.GetPlayer();
    std::vector<Asteroid>& asteroids = objectManager.GetAsteroids();
    std::vector<Projectile>& projectiles = objectManager.GetProjectiles();
    std::vector<PowerUp>& powerups = objectManager.GetPowerups();

    // Player vs Asteroids
    if (!isInvulnerable) {
        for (auto& asteroid : asteroids) {
            if (asteroid.IsActive() &&
                CheckCircleCollision(player.GetPosition(), PLAYER_HIT_RADIUS,
                    asteroid.GetPosition(), asteroid.GetRadius())) {
                if (!hasShield) {
                    player.LoseLife();
                }
                else {
                    isInvulnerable = true;
                    hasShield = false;
                }
                break;
            }
        }
    }

    // Projectiles vs Asteroids
    for (size_t i = 0; i < projectiles.size(); i++) {
        if (!projectiles[i].IsActive()) continue;

        for (size_t j = 0; j < asteroids.size(); j++) {
            if (!asteroids[j].IsActive()) continue;

            if (CheckCircleCollision(projectiles[i].GetPosition(), PROJECTILE_RADIUS,
                asteroids[j].GetPosition(), asteroids[j].GetRadius())) {
                gameScore.AddPoints(asteroids[j].GetPoints());
                HandleAsteroidDestruction(static_cast<int>(j));
                projectiles[i].Deactivate();
                break;
            }
        }
    }

    // Player vs Powerups
    for (auto& powerup : powerups) {
        if (powerup.IsActive() &&
            CheckCircleCollision(player.GetPosition(), PLAYER_HIT_RADIUS,
                powerup.GetPosition(), POWERUP_RADIUS)) {
            powerup.Collect();
        }
    }
}

void CollisionSystem::HandleAsteroidDestruction(int asteroidIndex) {
    std::vector<Asteroid>& asteroids = objectManager.GetAsteroids();
    Vector2 pos = asteroids[asteroidIndex].GetPosition();
    AsteroidSize size = asteroids[asteroidIndex].GetSize();

    asteroids[asteroidIndex].Destroy();

    if (size == LARGE) {
        objectManager.SpawnAsteroid({ pos.x + 20, pos.y }, MEDIUM);
        objectManager.SpawnAsteroid({ pos.x - 20, pos.y }, MEDIUM);
    }
    else if (size == MEDIUM) {
        objectManager.SpawnAsteroid({ pos.x + 10, pos.y }, SMALL);
        objectManager.SpawnAsteroid({ pos.x - 10, pos.y }, SMALL);
    }

    if ((rand() / static_cast<float>(RAND_MAX)) < POWERUP_SPAWN_RATE) {
        objectManager.SpawnPowerUp(pos);
    }
}

bool CollisionSystem::CheckCircleCollision(Vector2 pos1, float radius1,
    Vector2 pos2, float radius2) const {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return (dx * dx + dy * dy) < (radius1 + radius2) * (radius1 + radius2);
}