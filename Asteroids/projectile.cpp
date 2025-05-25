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
    DrawCircleV(position, 2, BLACK);
}

Rectangle Projectile::GetBounds() const {
    return { position.x - 2, position.y - 2, 4, 4 };
}