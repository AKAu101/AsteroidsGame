#pragma once
#include "raylib.h"

enum AsteroidSize {
    LARGE,
    MEDIUM,
    SMALL
};

class Asteroid {
public:
    Asteroid();
    void Spawn(Vector2 pos, AsteroidSize asteroidSize);
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const { return active; }
    Vector2 GetPosition() const { return position; }
    float GetRadius() const;
    AsteroidSize GetSize() const { return size; }
    int GetPoints() const;
    Rectangle GetBounds() const;
    void Destroy() { active = false; }

private:
    Vector2 position;
    Vector2 velocity;
    AsteroidSize size;
    float rotation;
    float rotationSpeed;
    bool active;
    int colorIndex; // New member for color variation
};