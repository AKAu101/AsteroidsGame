#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"

enum PowerUpType {
    EXTRA_LIFE,
    RAPID_FIRE,
    SHIELD
};

class PowerUp {
private:
    Vector2 position;
    PowerUpType type;
    float lifetime;
    bool active;
    float animationTimer;

public:
    PowerUp();
    void Spawn(Vector2 pos, PowerUpType powerType);
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const { return active; }
    Vector2 GetPosition() const { return position; }
    PowerUpType GetType() const { return type; }
    Rectangle GetBounds() const;
    void Collect() { active = false; }
};

#endif