// spaceship.h - Spielerklasse Definitionen
#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "raylib.h"

class Spaceship {
private:
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool isThrusting;
    int lives;
    bool invulnerable;
    float invulnerabilityTimer;
    float thrustCooldown;

public:
    Spaceship();

    // KEIN Input-Handling mehr - nur Zustandsänderungen
    void Update(float deltaTime);
    void Draw() const;  // const = ändert nichts

    // Öffentliche Aktionen (werden von Game aufgerufen)
    void StartThrust();
    void StopThrust();
    void ApplyThrust(float deltaTime);
    void Rotate(float direction, float deltaTime);

    void Reset();

    // Getters (alle const)
    Vector2 GetPosition() const { return position; }
    float GetRotation() const { return rotation; }
    Vector2 GetVelocity() const { return velocity; }
    bool IsInvulnerable() const { return invulnerable; }
    bool IsThrusting() const { return isThrusting; }
    int GetLives() const { return lives; }
    Rectangle GetBounds() const;

    // Spiellogik
    void LoseLife();
    void AddLife() { lives++; }
};

#endif