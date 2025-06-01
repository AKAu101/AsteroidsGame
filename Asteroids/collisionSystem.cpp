#include "collisionSystem.h"
#include "globals.h"

CollisionSystem::CollisionSystem(ObjectManager& objMgr, Score& score) :
    objectManager(objMgr),
    gameScore(score) {
}

void CollisionSystem::CheckCollisions(bool hasShield, bool isInvulnerable) {
    CheckProjectileAsteroidCollisions();
    CheckPlayerAsteroidCollisions(hasShield, isInvulnerable);
}

void CollisionSystem::CheckProjectileAsteroidCollisions() {
    // Verwende const_cast nur wenn nötig - aber sichere Implementierung
    auto& projectiles = const_cast<std::vector<Projectile>&>(objectManager.GetProjectiles());
    auto& asteroids = const_cast<std::vector<Asteroid>&>(objectManager.GetAsteroids());

    bool collisionFound = false;

    for (auto& projectile : projectiles) {
        if (!projectile.IsActive() || collisionFound) continue;

        for (auto& asteroid : asteroids) {
            if (!asteroid.IsActive()) continue;

            if (CheckCollisionRecs(projectile.GetBounds(), asteroid.GetBounds())) {
                // SOFORT beide Objekte deaktivieren um Mehrfachkollisionen zu verhindern
                projectile.Deactivate();

                // Daten vor Deaktivierung speichern
                Vector2 asteroidPos = asteroid.GetPosition();
                AsteroidSize currentSize = asteroid.GetSize();
                int points = asteroid.GetPoints();

                // Asteroid SOFORT deaktivieren
                asteroid.Destroy();

                // Punkte hinzufügen
                gameScore.AddPoints(points);

                // Kleinere Asteroiden spawnen - aber nur einmal!
                if (currentSize == LARGE) {
                    // Leicht versetzte Positionen um Überlappung zu vermeiden
                    Vector2 pos1 = { asteroidPos.x + 20, asteroidPos.y + 20 };
                    Vector2 pos2 = { asteroidPos.x - 20, asteroidPos.y - 20 };
                    objectManager.SpawnAsteroid(pos1, MEDIUM);
                    objectManager.SpawnAsteroid(pos2, MEDIUM);
                }
                else if (currentSize == MEDIUM) {
                    // Leicht versetzte Positionen um Überlappung zu vermeiden
                    Vector2 pos1 = { asteroidPos.x + 15, asteroidPos.y + 15 };
                    Vector2 pos2 = { asteroidPos.x - 15, asteroidPos.y - 15 };
                    objectManager.SpawnAsteroid(pos1, SMALL);
                    objectManager.SpawnAsteroid(pos2, SMALL);
                }
                // SMALL Asteroiden spawnen nichts

                // PowerUp-Spawn-Chance (nur bei großen Asteroiden)
                if (currentSize == LARGE && GetRandomValue(0, 100) < 20) { // 20% Chance
                    PowerUpType randomType = static_cast<PowerUpType>(GetRandomValue(0, 2));
                    objectManager.SpawnPowerUp(asteroidPos, randomType);
                }

                collisionFound = true;
                break; // Wichtig: Schleife verlassen nach Kollision
            }
        }

        if (collisionFound) break; // Äußere Schleife auch verlassen
    }
}

void CollisionSystem::CheckPlayerAsteroidCollisions(bool hasShield, bool isInvulnerable) {
    if (isInvulnerable) return; // Spieler ist unverwundbar - keine Kollisionen

    Spaceship& player = objectManager.GetPlayer();
    auto& asteroids = const_cast<std::vector<Asteroid>&>(objectManager.GetAsteroids());

    Rectangle playerBounds = player.GetBounds();

    for (auto& asteroid : asteroids) {
        if (!asteroid.IsActive()) continue;

        if (CheckCollisionRecs(playerBounds, asteroid.GetBounds())) {
            // Asteroid SOFORT deaktivieren
            asteroid.Destroy();

            // Prüfe ob Schild aktiv ist
            if (player.IsShieldActive()) {
                // Schild absorbiert den Schaden und wird zerstört
                player.DeactivateShield();
                return; // Spieler verliert kein Leben
            }

            // Spieler verliert ein Leben (nur wenn kein Schild und nicht unverwundbar)
            player.LoseLife();
            break; // Nur eine Kollision pro Frame
        }
    }
}