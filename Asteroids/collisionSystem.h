#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "objectmanager.h"
#include "score.h"
#include "raylib.h"

/**
 * CollisionSystem class handles all collision detection and response in the game
 * Manages interactions between projectiles, asteroids, and the player spaceship
 * Integrates with scoring system and object management for complete collision handling
 */
class CollisionSystem {
private:
    ObjectManager& objectManager;  // Reference to object manager for accessing game objects
    Score& gameScore;             // Reference to score system for awarding points

    void CheckProjectileAsteroidCollisions();
    void CheckPlayerAsteroidCollisions(bool hasShield, bool isInvulnerable);

public:
      CollisionSystem(ObjectManager& objMgr, Score& score);
    void CheckCollisions(bool hasShield, bool isInvulnerable);
};

#endif