#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "objectmanager.h"
#include "score.h"

class CollisionSystem {
private:
    ObjectManager& objectManager;
    Score& gameScore;

public:
    CollisionSystem(ObjectManager& objManager, Score& score);

    void CheckCollisions(bool& hasShield, bool isInvulnerable);
    void HandleAsteroidDestruction(int asteroidIndex);

private:
    bool CheckCircleCollision(Vector2 pos1, float radius1,
        Vector2 pos2, float radius2) const;
};

#endif