#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "objectmanager.h"
#include "score.h"
#include "raylib.h"

class CollisionSystem {
private:
    ObjectManager& objectManager;
    Score& gameScore;

    // Private Hilfsmethoden
    void CheckProjectileAsteroidCollisions();
    void CheckPlayerAsteroidCollisions(bool hasShield, bool isInvulnerable);

public:
    CollisionSystem(ObjectManager& objMgr, Score& score);

    // Hauptmethode - Signatur muss mit game.cpp übereinstimmen
    void CheckCollisions(bool hasShield, bool isInvulnerable);
};

#endif