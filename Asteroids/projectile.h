#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

class Projectile {
private:
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    bool active;

public:
    Projectile();
    void Fire(Vector2 startPos, float rotation);
    void Update(float deltaTime);
    void Draw() const;  // Made const
    bool IsActive() const { return active; }
    Vector2 GetPosition() const { return position; }
    Rectangle GetBounds() const;
    void Deactivate() { active = false; }
};

#endif