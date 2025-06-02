#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"

enum PowerUpType {
    EXTRA_LIFE,     // Adds one extra life to the player
    RAPID_FIRE,     // Enables rapid fire mode for 50 shots
    SHIELD          // Activates protective shield for 8 seconds
};

class PowerUp {
private:
    Vector2 position;           // Current x,y coordinates of power-up center
    PowerUpType type;           // Type of power-up (EXTRA_LIFE, RAPID_FIRE, SHIELD)
    float lifetime;             // Remaining time in seconds before power-up expires
    bool active;                // Whether power-up is alive and should be processed
    float animationTimer;       // Timer for visual animation effects

public:
    PowerUp();

    void Spawn(Vector2 pos /* Starting position coordinates */,
        PowerUpType powerType /* Type of power-up to create */);
    void Update(float deltaTime);
    void Draw() const;

    bool IsActive() const {
        return active;
    }
    Vector2 GetPosition() const {
        return position;
    }
    PowerUpType GetType() const {
        return type; 
    }
    Rectangle GetBounds() const;
    void Collect() {
        active = false;
    }
};

#endif