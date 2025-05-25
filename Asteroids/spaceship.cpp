#include "spaceship.h"
#include "globals.h"
#include <cmath>

Spaceship::Spaceship() {
    Reset();
    lives = STARTING_LIVES;
}

void Spaceship::Reset() {
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    velocity = { 0, 0 };
    rotation = 0;
    isThrusting = false;
    invulnerable = false;
    invulnerabilityTimer = 0.0f;
    thrustCooldown = 0;
    
    // Dreiecksparameter
    frontDistance = 15.0f;
    sideDistance = 10.0f;
    angleOffset = 30.0f;
}

void Spaceship::Update(float deltaTime) {
    // Physik anwenden
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Screen wrapping
    if (position.x < 0) position.x = SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH) position.x = 0;
    if (position.y < 0) position.y = SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT) position.y = 0;

    // Reibung anwenden
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Unverwundbarkeit updaten
    if (invulnerable) {
        invulnerabilityTimer -= deltaTime;
        if (invulnerabilityTimer <= 0) {
            invulnerable = false;
        }
    }

    // Cooldowns updaten
    if (thrustCooldown > 0) {
        thrustCooldown -= deltaTime;
    }
    
    // Dreieckspunkte aktualisieren
    UpdateTrianglePoints();
}

void Spaceship::UpdateTrianglePoints() {
    // Aktualisiere die Dreieckspunkte basierend auf aktueller Position und Rotation
    float radRotation = rotation * WINKEL2GRAD;
    
    front = {
        position.x + cos(radRotation) * frontDistance,
        position.y + sin(radRotation) * frontDistance
    };

    left = {
        position.x + cos(radRotation + (180 - angleOffset) * WINKEL2GRAD) * sideDistance,
        position.y + sin(radRotation + (180 - angleOffset) * WINKEL2GRAD) * sideDistance
    };

    right = {
        position.x + cos(radRotation + (180 + angleOffset) * WINKEL2GRAD) * sideDistance,
        position.y + sin(radRotation + (180 + angleOffset) * WINKEL2GRAD) * sideDistance
    };
}

void Spaceship::StartThrust() {
    isThrusting = true;
}

void Spaceship::StopThrust() {
    isThrusting = false;
}

void Spaceship::ApplyThrust(float deltaTime) {
    if (!isThrusting) return;
    
    // 1. Basis-Mitte berechnen (virtueller Punkt zwischen left/right)
    Vector2 baseMid = {
        (left.x + right.x) * 0.5f,
        (left.y + right.y) * 0.5f
    };

    // 2. Schubrichtung: Von Basis-Mitte zur Spitze
    Vector2 thrustDir = {
        front.x - baseMid.x,
        front.y - baseMid.y
    };

    // 3. Normalisiere den Richtungsvektor
    float thrustLength = sqrt(thrustDir.x * thrustDir.x + thrustDir.y * thrustDir.y);
    if (thrustLength > 0) {
        thrustDir.x /= thrustLength;
        thrustDir.y /= thrustLength;
    }

    // 4. Beschleunigung anwenden
    velocity.x += thrustDir.x * SPACESHIP_ACCELERATION * deltaTime;
    velocity.y += thrustDir.y * SPACESHIP_ACCELERATION * deltaTime;

    // 5. Geschwindigkeit begrenzen
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > MAX_SPACESHIP_SPEED) {
        velocity.x = (velocity.x / speed) * MAX_SPACESHIP_SPEED;
        velocity.y = (velocity.y / speed) * MAX_SPACESHIP_SPEED;
    }
}

void Spaceship::Rotate(float direction, float deltaTime) {
    rotation += direction * SPACESHIP_ROTATION_SPEED * deltaTime;
}

void Spaceship::Draw() const {
    // Flackern nur wenn unverwundbar
    if (invulnerable && ((int)(invulnerabilityTimer * 10) % 2)) {
        return;
    }

    // Dreieck zeichnen (offene Basis)
    DrawLineV(front, left, WHITE);
    DrawLineV(front, right, WHITE);
    
    // Optional: Punkt an der Spitze markieren
    DrawCircleV(front, 3, RED);

    // Schub-Effekt zeichnen
    if (isThrusting) {
        Vector2 baseMid = {
            (left.x + right.x) * 0.5f,
            (left.y + right.y) * 0.5f
        };
        DrawLineV(left, baseMid, ORANGE);
        DrawLineV(right, baseMid, ORANGE);
    }
}

void Spaceship::LoseLife() {
    lives--;
    invulnerable = true;
    invulnerabilityTimer = 3.0f;
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    velocity = { 0, 0 };
    rotation = 0;
    isThrusting = false;
}

Rectangle Spaceship::GetBounds() const {
    return { position.x - frontDistance, position.y - frontDistance, 
             frontDistance * 2, frontDistance * 2 };
}