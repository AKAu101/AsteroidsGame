#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "raylib.h"

class Spaceship {
private:
    // Core variables
    Vector2 position;               // Current x,y coordinates of spaceship center
    Vector2 velocity;               // Movement speed and direction (pixels per second)
    float rotation;                 // Current rotation angle in degrees
    float triangleSize;             // Radius/size of the triangular spaceship

    // State variables
    bool isThrusting;               // Whether thrust is currently being applied
    bool invulnerable;              // Whether spaceship is immune to damage
    int lives;                      // Number of remaining lives
    float invulnerabilityTimer;     // Remaining invulnerability time in seconds
    float thrustCooldown;           // Cooldown timer for thrust effects

    // Shield variables
    bool shieldActive;              // Whether protective shield is currently active
    float shieldTimer;              // Remaining shield time in seconds
    float shieldAnimationTimer;     // Timer for shield visual animation effects

    // Triangle geometry
    Vector2 trianglePoints[3];      // Triangle vertices (0=tip, 1=left, 2=right)

    void UpdateTriangleGeometry();

public:
    Spaceship();

    // Core methods
    void Update(float deltaTime /* Time elapsed since last frame in seconds */);
    void Draw() const;
    void Reset();

    // Control
    void StartThrust();
    void StopThrust();
    void ApplyThrust(float deltaTime);
    void Rotate(float direction /* Rotation direction (-1 for left, +1 for right) */,
        float deltaTime);

    // Status queries
    Vector2 GetPosition() const;
    float GetRotation() const;
    Vector2 GetVelocity() const;
    void setInvulnerableTimer(float value /* Invulnerability duration in seconds */);
    void setIsInvulnerable(bool value /* New invulnerability state */);
    bool IsInvulnerable() const;
    float GetInvulnerabilityTime() const;  // New method
    bool IsThrusting() const;
    int GetLives() const;
    Rectangle GetBounds() const;

    // Shield methods
    void ActivateShield();
    void DeactivateShield();
    bool IsShieldActive() const;
    float GetShieldTimeRemaining() const;

    // Life management
    void LoseLife();
    void AddLife();
};

#endif