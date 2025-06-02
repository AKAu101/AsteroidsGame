#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

class Projectile {
private:
    Vector2 position;       // Current x,y coordinates of projectile center
    Vector2 velocity;       // Movement speed and direction (pixels per second)
    float lifetime;         // Remaining time in seconds before projectile expires
    bool active;            // Whether projectile is alive and should be processed

public:
    Projectile();

    void Fire(Vector2 startPos /* Starting position coordinates */,
        float rotation /* Direction angle in degrees */);
    void Update(float deltaTime);
    void Draw() const;

    bool IsActive() const {
        return active;
    }
    Vector2 GetPosition() const {
        return position;
    }
    Rectangle GetBounds() const;
    void Deactivate() {
        active = false;
    }
};

#endif