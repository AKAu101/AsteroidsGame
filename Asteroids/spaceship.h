#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "raylib.h"

class Spaceship {
private:
    // Kernvariablen
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float triangleSize;

    // Zustandsvariablen
    bool isThrusting;
    bool invulnerable;
    int lives;
    float invulnerabilityTimer;
    float thrustCooldown;

    // Schild-Variablen
    bool shieldActive;
    float shieldTimer;
    float shieldAnimationTimer;

    // Dreiecksgeometrie
    Vector2 trianglePoints[3]; // 0=Spitze, 1=Links, 2=Rechts

    void UpdateTriangleGeometry();

public:
    Spaceship();

    // Kernmethoden
    void Update(float deltaTime);
    void Draw() const;
    void Reset();

    // Steuerung
    void StartThrust();
    void StopThrust();
    void ApplyThrust(float deltaTime);
    void Rotate(float direction, float deltaTime);

    // Statusabfragen
    Vector2 GetPosition() const;
    float GetRotation() const;
    Vector2 GetVelocity() const;
    void setInvulnerableTimer(float value);
    void setIsInvulnerable(bool value);
    bool IsInvulnerable() const;
    float GetInvulnerabilityTime() const;  // Neue Methode
    bool IsThrusting() const;
    int GetLives() const;
    Rectangle GetBounds() const;

    // Schild-Methoden
    void ActivateShield();
    void DeactivateShield();
    bool IsShieldActive() const;
    float GetShieldTimeRemaining() const;

    // Lebensmanagement
    void LoseLife();
    void AddLife();
};

#endif