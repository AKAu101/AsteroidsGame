#include "projectile.h"
#include "globals.h"
#include <cmath>

Projectile::Projectile() {
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    lifetime = 0.0f;
    active = false;
}

void Projectile::Fire(Vector2 startPos, float rotation) {
    position = startPos;
    float radians = rotation * WINKEL2GRAD;
    velocity.x = cos(radians) * PROJECTILE_SPEED;
    velocity.y = sin(radians) * PROJECTILE_SPEED;
    lifetime = PROJECTILE_LIFETIME;
    active = true;
}

void Projectile::Update(float deltaTime) {
    if (!active) return;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    lifetime -= deltaTime;

    // Screen wrapping
    if (position.x < 0) position.x = SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH) position.x = 0;
    if (position.y < 0) position.y = SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT) position.y = 0;

    if (lifetime <= 0) {
        active = false;
    }
}

void Projectile::Draw() const {
    if (!active) return;

    // Hochkantiges, schmales Rechteck (3 Pixel breit, 12 Pixel hoch)
    Rectangle rect = {
        position.x - 1.5f,  // Zentriert (1.5 Pixel auf jeder Seite)
        position.y - 6.0f,   // 6 Pixel nach oben (halbe Höhe)
        3.0f,               // Breite
        12.0f               // Höhe
    };

    // Optional: Ausrichtung in Bewegungsrichtung
    float angle = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f; // +90° für Hochkant

    DrawRectanglePro(
        rect,
        Vector2{ 1.5f, 6.0f },  // Pivotpunkt in der Mitte
        angle,
        BLACK
    );
}

Rectangle Projectile::GetBounds() const {
    return { position.x - 2, position.y - 2, 4, 4 };
}