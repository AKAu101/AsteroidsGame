#pragma once
#include "raylib.h"

/**
 * Enumeration defining the different size categories for asteroids
 * Determines visual size, collision radius, point values, and splitting behavior
 */
enum AsteroidSize {
    LARGE,   // Largest asteroids, spawn at game start and from edges
    MEDIUM,  // Medium-sized pieces created when large asteroids are destroyed
    SMALL    // Smallest fragments, completely destroyed when hit
};

/**
 * Asteroid class representing destructible space rocks
 * Features progressive difficulty scaling, visual variety, and physics simulation
 * Supports three size categories with different behaviors and point values
 */
class Asteroid {
public:
    Asteroid();
    void Spawn(Vector2 pos, AsteroidSize asteroidSize);
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const {
        return active;
    }
    Vector2 GetPosition() const { 
        return position;
    }
    float GetRadius() const;
    AsteroidSize GetSize() const { 
        return size;
    }
    int GetPoints() const;
    Rectangle GetBounds() const;
    void Destroy() { 
        active = false;
    }

private:
    Vector2 position;        // Current x,y coordinates of asteroid center
    Vector2 velocity;        // Movement speed and direction (pixels per second)
    AsteroidSize size;       // Size category determining radius and behavior
    float rotation;          // Current rotation angle in degrees
    float rotationSpeed;     // Angular velocity in degrees per second
    bool active;             // Whether asteroid is alive and should be processed
    int colorIndex;          // Index into color palette for visual variety
};